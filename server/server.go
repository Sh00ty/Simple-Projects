package main

import (
	"fmt"
	"log"
	"net/http"
	"runtime/debug"
)

// ErrorLoging----------------------------------------------------------ErrorLoging

type application struct {
	errorLog *log.Logger
	infoLog  *log.Logger
}

func (app *application) serverError(w http.ResponseWriter, err error) {
	trace := fmt.Sprintf("%s\n%s", err.Error(), debug.Stack())
	app.errorLog.Println(trace)
	http.Error(w, http.StatusText(http.StatusInternalServerError), http.StatusInternalServerError)
}

func (app *application) clientError(w http.ResponseWriter, status int) {
	http.Error(w, http.StatusText(status), status)
}

func (app *application) notFound(w http.ResponseWriter) {
	app.clientError(w, http.StatusNotFound)
}

//server------------------------------------------------------------------server

func (app *application) mainPage(w http.ResponseWriter, r *http.Request) {

}

/*
func main() {
	infoLog := log.New(os.Stdout, "Info::\t", log.Ldate|log.Ltime)
	errLog := log.New(os.Stderr, "Error::\t", log.Ldate|log.Ltime|log.Lshortfile)

	app := &application{errLog, infoLog}

	mux := http.NewServeMux()
	mux.HandleFunc("/", app.mainPage)
	server := http.Server{
		Addr:         ":8080",
		Handler:      mux,
		ReadTimeout:  10 * time.Second,
		WriteTimeout: 10 * time.Second,
	}
	infoLog.Println("started")
	err := server.ListenAndServe()
	log.Fatal(err)
}
*/
