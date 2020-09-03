package main

import (
	"time"
)

type StopWatch struct {
	startTime int64
	endTime   int64
}

func NewStopWatch() *StopWatch {
	return &StopWatch{startTime: 0, endTime: 0}
}

func (s *StopWatch) Start() {
	s.startTime = time.Now().UnixNano()
	s.endTime = s.startTime
}

func (s *StopWatch) Stop() {
	s.endTime = time.Now().UnixNano()
}

func (s *StopWatch) GetElapsedTimestamp() int64 {
	return (s.endTime - s.startTime)
}

func (s *StopWatch) GetElapsedSeconds() float64 {
	return (float64(s.endTime-s.startTime) / 1e9)
}

func (s *StopWatch) GetElapsedMillisecs() float64 {
	return (float64(s.endTime-s.startTime) / 1e6)
}

func (s *StopWatch) GetElapsedMicrosecs() float64 {
	return (float64(s.endTime-s.startTime) / 1e3)
}

func (s *StopWatch) GetElapsedNanosecs() float64 {
	return float64(s.endTime - s.startTime)
}
