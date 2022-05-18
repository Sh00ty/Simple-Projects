package main

import (
	"bytes"
	"crypto/rand"
	"crypto/sha1"
	"encoding/binary"
	"fmt"
	"io"
	"log"
	"net"
	"net/http"
	"net/url"
	"os"
	"strconv"
	"strings"
	"sync"
	"sync/atomic"
	"time"

	//"github.com/jackpal/bencode-go"
	bencode "github.com/jackpal/bencode-go"
)

const hashlen = 20

// structs for bencode Unmarshal----------------------------------------------structs for bencode Unmarshal

// for torrent with many files--------------------------------for torrent with many files
type MultipleBencodeFile struct {
	Info         info     `bencode:"info"`
	Announce     string   `bencode:"announce"`
	AnnounceList []string `bencode:"announce-list"`
	Files        files    `bencode:"files"`
	Length       int      `bencode:"length"`
}

type files struct {
	Length int      `bencode:"file length"`
	Path   []string `bencode:"Path"`
}

// for torrent with one file--------------------------------for torrent with one file
type SingleBencodeFile struct {
	Info         info       `bencode:"info"`
	Announce     string     `bencode:"announce"`
	AnnounceList [][]string `bencode:"announce-list"`
	CreatedBy    string     `bencode:"created by"`
	Comment      string     `bencode:"comment"`
	CreationDate uint64     `bencode:"creation date"`
}

// INFO_PIECE----------------------------------------------------------------INFO PIECE
type info struct {
	Pieces      string `bencode:"pieces"`
	PieceLength int    `bencode:"piece length"`
	Name        string `bencode:"name"`
	Length      int    `bencode:"length"`
}

func (inf info) makeInfoHash(t *TorrentFile, wg *sync.WaitGroup) {
	var bytes bytes.Buffer
	err := bencode.Marshal(&bytes, inf)
	if err != nil {
		wg.Done()
		return
	}
	t.InfoHash = sha1.Sum(bytes.Bytes())
	wg.Done()
}

// Torrent files interface----------------------------------------------------------------Torrent files interface
type TorrentFiles interface {
	infoHash() *[hashlen]byte
	length() int
	announce() string
}

func trakerUrl(t TorrentFiles, announce string, peerId [20]byte, port int) (string, error) {
	Url, err := url.Parse(announce)
	if err != nil {
		return "", fmt.Errorf("trakerUrl : %w", err)
	}
	params := url.Values{
		"info_hash":  []string{string(t.infoHash()[:])},
		"peer_id":    []string{string(peerId[:])},
		"port":       []string{strconv.Itoa(port)},
		"uploaded":   []string{"0"},
		"downloaded": []string{"0"},
		"compact":    []string{"1"},
		"left":       []string{strconv.Itoa(t.length())},
	}

	Url.RawQuery = params.Encode()
	return Url.String(), nil
}

// TorrentFile(single)----------------------------------------------------------------TorrentFile
type TorrentFile struct {
	Name           string
	Length         int
	InfoHash       [hashlen]byte
	CreationDate   uint64
	Pieces         [][hashlen]byte
	PieceLength    int
	Announce       []string
	AnnounceNumber int
	CreatedBy      string
	Comment        string

	path string
}

func (t TorrentFile) infoHash() *[hashlen]byte {
	return &t.InfoHash
}

func (t *TorrentFile) announce() string {
	if t.AnnounceNumber >= len(t.Announce) {
		return ""
	}
	t.AnnounceNumber++
	return t.Announce[t.AnnounceNumber-1]
}

func (t TorrentFile) length() int {
	return t.Length
}

func (t *TorrentFile) makeSingle() error {
	file, err := os.Open(t.path)
	if err != nil {
		return fmt.Errorf("make Single : %w", err)
	}
	defer file.Close()

	single := new(SingleBencodeFile)
	err = bencode.Unmarshal(file, single)
	if err != nil {
		return fmt.Errorf("make Single : %w", err)
	}

	t.Name = single.Info.Name
	t.Length = single.Info.Length
	t.PieceLength = single.Info.PieceLength
	t.CreationDate = single.CreationDate
	t.CreationDate = single.CreationDate
	t.CreatedBy = single.CreatedBy
	t.Comment = single.Comment
	t.Pieces = make([][hashlen]byte, t.PieceLength)
	t.Announce = append(t.Announce, single.Announce)
	for _, v := range single.AnnounceList {
		t.Announce = append(t.Announce, v[0])
	}
	t.AnnounceNumber = 0

	if single.Info.PieceLength == 0 || len(single.Info.Pieces) == 0 {
		return fmt.Errorf("make Single : %s", "torrent file is broken or empty")
	}

	if len(single.Info.Pieces)%hashlen != 0 {
		return fmt.Errorf("make Single : hash count mod hashlen != 0 ")
	}

	// i'll make it parallel for speed
	var wg sync.WaitGroup
	wg.Add(1)
	go single.Info.makeInfoHash(t, &wg)

	for i := 0; i < len(single.Info.Pieces); i++ {
		t.Pieces[i/hashlen][i%hashlen] = single.Info.Pieces[i]
	}
	wg.Wait()

	if t.InfoHash == [20]byte{} {
		return fmt.Errorf("make Single : makeInfoHash : cant marshal info block to string")
	}
	return nil
}

// peer------------------------------------------------------------------------peer

type peer struct {
	ip   net.IP
	port uint16
}

func (p peer) String() string {
	return net.JoinHostPort(p.ip.String(), strconv.Itoa(int(p.port)))
}

type Peers struct {
	Peers   chan peer
	Timeout time.Duration
}

// goForPeers----------------------------------------------------------------goForPeers
type trakerPeerAws struct {
	Interval int    `bencode:"interval"`
	Peers    string `bencode:"peers"`
}

func (p *Peers) goForPeers(t TorrentFiles, peerId [20]byte, port int) {
	var counter int32 = 0
	for i := 0; i >= 0; i++ {
		ann := t.announce()
		fmt.Println(ann)
		if ann == "" {
			break
		}
		Url, err := trakerUrl(t, t.announce(), peerId, port+i)
		if err != nil {
			log.Println(fmt.Errorf("goForPeers : %w", err))
		}
		atomic.AddInt32(&counter, 1)
		go DoRequest(p, Url, &counter)
	}
}

func deferFuncForDoRequest(p *Peers, counter *int32) {
	atomic.AddInt32(counter, -1)
	if *counter == 0 {
		fmt.Println("chanal closed")
		close(p.Peers)
	}
}

func DoRequest(p *Peers, Url string, counter *int32) {
	defer deferFuncForDoRequest(p, counter)
	fmt.Println(Url)
	req := &http.Request{
		Method: http.MethodGet,
		Header: http.Header{
			"User-Agent":      {"Mozilla/5.0 (Windows NT 5.01) AppleWebKit/5340 (KHTML, like Gecko) Chrome/39.0.842.0 Mobile Safari/5340"},
			"Accept-Encoding": {"gzip"},
			"Connection":      {"close"},
		},
	}
	req.URL, _ = url.Parse(Url)
	client := &http.Client{Timeout: 20 * time.Second}

	resp, err := client.Do(req)
	if err != nil {
		log.Println(fmt.Errorf("goForPeers : cant download peers : %s", err.Error()[:strings.Index(err.Error(), "?")]))
		return
	}
	defer resp.Body.Close()

	if resp.StatusCode/100 != 2 && resp.StatusCode/100 != 3 {
		log.Println(fmt.Errorf("goForPeers : cant download peers : responce status code : %d", resp.StatusCode))
		return
	}

	var respDecoded trakerPeerAws
	err = bencode.Unmarshal(resp.Body, &respDecoded)
	if err != nil {
		log.Println(fmt.Errorf("goForPeers : %w", err))
		return
	}
	newPeers := respDecoded.pMakeSlice()
	if len(newPeers) == 0 {
		log.Println(fmt.Errorf("goForPeers : makeSlice : cant read peers ip and port"))
		return
	}

	for _, v := range newPeers {
		p.Peers <- v
	}
}

func (t trakerPeerAws) pMakeSlice() []peer {
	peerLen := 6
	if len(t.Peers)%peerLen != 0 {
		return []peer{}
	}

	peers := make([]peer, len(t.Peers)/peerLen)
	for i := 0; i < len(t.Peers)/peerLen; i += peerLen {
		peers[i/peerLen].ip = net.IP(t.Peers[i : i+4])
		peers[i/peerLen].port = binary.BigEndian.Uint16([]byte(t.Peers[i+4 : i+6]))
		fmt.Println(binary.BigEndian.Uint16([]byte(t.Peers[i+4 : i+6])))
	}
	return peers
}

// handshake------------------------------------------------------------------handshake

func startDeal(p peer, timeout time.Duration) (net.Conn, error) {
	conn, err := net.DialTimeout("tcp", p.String(), timeout)
	if err != nil {
		return nil, fmt.Errorf("startDeal : %w", err)
	}
	return conn, nil
}

type HandShake struct {
	protocol string
	infoHash [20]byte
	peerId   [20]byte
}

func HParse(r io.Reader) (*HandShake, error) {
	buf := make([]byte, 1)
	_, err := r.Read(buf)
	if err != nil {
		return nil, fmt.Errorf("cant parse responce handshake(Parse) : %w", err)
	}
	len := int(buf[0])
	if len == 0 {
		return nil, fmt.Errorf("Parse : len of pesponse != n")
	}
	buf = make([]byte, len+48)

	_, err = r.Read(buf)
	if err != nil {
		return nil, fmt.Errorf("cant parse responce handshake(Parse) : %w", err)
	}
	protocol := string(buf[0:len])
	var infoHash, peerId [20]byte
	copy(infoHash[:], buf[len+8:len+28])
	copy(peerId[:], buf[len+28:])

	h := &HandShake{
		infoHash: infoHash,
		protocol: protocol,
		peerId:   peerId,
	}
	return h, nil
}

func (h *HandShake) doHandShake(conn net.Conn) (*HandShake, error) {
	conn.SetDeadline(time.Now().Add(3 * time.Second))
	defer conn.SetDeadline(time.Time{})

	_, err := conn.Write(h.ToByteSlice())
	if err == nil {
		return nil, fmt.Errorf("doHandShake : %w", err)
	}

	respHandShake, err := HParse(conn)
	if err != nil {
		return nil, fmt.Errorf("doHandShake : %w", err)
	}

	if bytes.Equal(respHandShake.infoHash[:], h.infoHash[:]) {
		return respHandShake, nil
	}
	return nil, fmt.Errorf("wrong handshake, hashs are not equal")
}

func makeHandShake(peerId, infoHash [20]byte) HandShake {
	return HandShake{
		infoHash: infoHash,
		peerId:   peerId,
		protocol: "BitTorrent protocol",
	}
}

func (h HandShake) ToByteSlice() []byte {
	res := make([]byte, len(h.protocol)+49)
	res[0] = byte(len(h.protocol))
	now := 1
	now += copy(res[now:], h.protocol)
	// reserved bytes
	now += copy(res[now:], make([]byte, 8))
	now += copy(res[now:], h.infoHash[:])
	now += copy(res[now:], h.peerId[:])
	return res
}

//message----------------------------------------------------------------------message
type messageId uint8

const (
	mChoke         messageId = 0
	mUnchoke       messageId = 1
	mInterested    messageId = 2
	mNotInterested messageId = 3
	mHave          messageId = 4
	mBitfield      messageId = 5
	mRequest       messageId = 6
	mPiece         messageId = 7
	mCancel        messageId = 8
)

type message struct {
	Id      messageId
	PayLoad []byte
}

func MMakeSlice() {}

func MParse(r io.Reader) (*message, error) {
	buf := make([]byte, 4)
	_, err := r.Read(buf)
	if err != nil {
		return nil, fmt.Errorf("cant parse responce message(Parse) : ", err)
	}
	len := int(binary.BigEndian.Uint32(buf[:]))
	if len == 0 {
		// keep alive message (spisal)
		return nil, nil
	}

	buf = make([]byte, len)

	_, err = r.Read(buf)
	if err != nil {
		return nil, fmt.Errorf("cant parse responce message(Parse) : ", err)
	}

	m := &message{
		Id:      messageId(buf[0]),
		PayLoad: buf[1:],
	}
	return m, nil
}

var path string = "/Users/pavelshykov/programming/Myprojects/server/spider-man__far_from_home_2019_webrip_1080p.torrent"

func main() {
	//fmt.Println(len("?compact=1&downloaded=0&info_hash=%B6%0Bv%99%91%21%C3%5CnY%A6%3EI%F2%8Ci%D6~5+&left=7302446693&peer_id=%A7%BDR2%7F%B4~%13%25%27%CCk%070z%CD%9Cv%09U&port=7107&uploaded=0"))
	file := new(TorrentFile)
	file.path = path
	err := file.makeSingle()
	// var sx16 string = fmt.Sprintf("%x", file.InfoHash)
	// fmt.Println(sx16)
	if err != nil {
		fmt.Println(err)
	}
	//file.Announce = "http://tracker.tiny-vps.com:6969/announce"

	fmt.Println(fmt.Sprintf("%x", file.InfoHash))

	auto := Peers{
		Timeout: 10 * time.Second,
	}

	auto.Peers = make(chan peer)

	var peerID [20]byte
	_, err = rand.Read(peerID[:])
	var port int = 6969

	auto.goForPeers(file, peerID, port)

	for {
		if value, ok := <-auto.Peers; ok {
			con, err := startDeal(value, auto.Timeout)
			if err == nil {
				fmt.Println("start handshake")
				h := makeHandShake(peerID, file.InfoHash)
				h2, err := h.doHandShake(con)
				if err != nil {
					log.Println(err)
				}
				fmt.Println("handshake : ", h2.infoHash)
			}
			log.Println(err)
		} else {
			break
		}
	}
	//GET http://tracker.dler.org:6969/announce?info_hash=%c7%ca_l%83%f1%80%d2%1e%1d%7c%fdz%ac%40%faNm%22%82&peer_id=-LT10B0-~2KwgouEKLVz&port=14082&uploaded=0&downloaded=0&left=754687337&corrupt=0&key=F25F6675&event=started&numwant=200&compact=1&no_peer_id=1&supportcrypto=1&redundant=0 HTTP/1.1

	//magnet:?xt=urn:btih:c7ca5f6c83f180d21e1d7cfd7aac40fa4e6d2282&dn=Spider-Man.%20Far%20from%20Home%20(2019)%20WEBRip%20%5B1080p%5D%20Trailer%20%E2%84%962.mkv&tr=http://retracker.local/announce&tr=http://tracker.filetracker.pl:8089/announce&tr=http://tracker2.wasabii.com.tw:6969/announce&tr=http://tracker.grepler.com:6969/announce&tr=http://125.227.35.196:6969/announce&tr=http://tracker.tiny-vps.com:6969/announce&tr=http://87.248.186.252:8080/announce&tr=http://210.244.71.25:6969/announce&tr=http://46.4.109.148:6969/announce&tr=udp://46.148.18.250:2710&tr=http://tracker.dler.org:6969/announce&tr=udp://[2001:67c:28f8:92::1111:1]:2710&tr=udp://ipv6.leechers-paradise.org:6969

}
