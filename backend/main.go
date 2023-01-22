package main

import (
	"encoding/json"
	"time"

	"net/http"

	"github.com/kerplunkx/people-counter/mqtt"
)

type Data struct {
	Count string    `json:"count"`
	Date  time.Time `json:"time"`
}

var data []Data

func main() {
	go mqtt.GetCount()
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Content-Type", "application/json")
		if r.Method == http.MethodGet {
			data = append(data, Data{Count: mqtt.Message, Date: time.Now()})
			json.NewEncoder(w).Encode(data)
		}
	})
	http.ListenAndServe(":8080", nil)
	time.Now()
}
