// Code generated by mockery v2.12.1. DO NOT EDIT.

package mocks

import (
	context "context"

	mock "github.com/stretchr/testify/mock"
	mongo "go.mongodb.org/mongo-driver/mongo"

	testing "testing"
)

// CollectionHelper is an autogenerated mock type for the CollectionHelper type
type CollectionHelper struct {
	mock.Mock
}

// DeleteOne provides a mock function with given fields: ctx, filter
func (_m *CollectionHelper) DeleteOne(ctx context.Context, filter interface{}) (*mongo.DeleteResult, error) {
	ret := _m.Called(ctx, filter)

	var r0 *mongo.DeleteResult
	if rf, ok := ret.Get(0).(func(context.Context, interface{}) *mongo.DeleteResult); ok {
		r0 = rf(ctx, filter)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(*mongo.DeleteResult)
		}
	}

	var r1 error
	if rf, ok := ret.Get(1).(func(context.Context, interface{}) error); ok {
		r1 = rf(ctx, filter)
	} else {
		r1 = ret.Error(1)
	}

	return r0, r1
}

// Find provides a mock function with given fields: ctx, filter
func (_m *CollectionHelper) Find(ctx context.Context, filter interface{}) (*mongo.Cursor, error) {
	ret := _m.Called(ctx, filter)

	var r0 *mongo.Cursor
	if rf, ok := ret.Get(0).(func(context.Context, interface{}) *mongo.Cursor); ok {
		r0 = rf(ctx, filter)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(*mongo.Cursor)
		}
	}

	var r1 error
	if rf, ok := ret.Get(1).(func(context.Context, interface{}) error); ok {
		r1 = rf(ctx, filter)
	} else {
		r1 = ret.Error(1)
	}

	return r0, r1
}

// FindOne provides a mock function with given fields: ctx, filter
func (_m *CollectionHelper) FindOne(ctx context.Context, filter interface{}) *mongo.SingleResult {
	ret := _m.Called(ctx, filter)

	var r0 *mongo.SingleResult
	if rf, ok := ret.Get(0).(func(context.Context, interface{}) *mongo.SingleResult); ok {
		r0 = rf(ctx, filter)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(*mongo.SingleResult)
		}
	}

	return r0
}

// InsertOne provides a mock function with given fields: ctx, filter
func (_m *CollectionHelper) InsertOne(ctx context.Context, filter interface{}) (interface{}, error) {
	ret := _m.Called(ctx, filter)

	var r0 interface{}
	if rf, ok := ret.Get(0).(func(context.Context, interface{}) interface{}); ok {
		r0 = rf(ctx, filter)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(interface{})
		}
	}

	var r1 error
	if rf, ok := ret.Get(1).(func(context.Context, interface{}) error); ok {
		r1 = rf(ctx, filter)
	} else {
		r1 = ret.Error(1)
	}

	return r0, r1
}

// ReplaceOne provides a mock function with given fields: ctx, filter, replacement
func (_m *CollectionHelper) ReplaceOne(ctx context.Context, filter interface{}, replacement interface{}) (*mongo.UpdateResult, error) {
	ret := _m.Called(ctx, filter, replacement)

	var r0 *mongo.UpdateResult
	if rf, ok := ret.Get(0).(func(context.Context, interface{}, interface{}) *mongo.UpdateResult); ok {
		r0 = rf(ctx, filter, replacement)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(*mongo.UpdateResult)
		}
	}

	var r1 error
	if rf, ok := ret.Get(1).(func(context.Context, interface{}, interface{}) error); ok {
		r1 = rf(ctx, filter, replacement)
	} else {
		r1 = ret.Error(1)
	}

	return r0, r1
}

// NewCollectionHelper creates a new instance of CollectionHelper. It also registers the testing.TB interface on the mock and a cleanup function to assert the mocks expectations.
func NewCollectionHelper(t testing.TB) *CollectionHelper {
	mock := &CollectionHelper{}
	mock.Mock.Test(t)

	t.Cleanup(func() { mock.AssertExpectations(t) })

	return mock
}
