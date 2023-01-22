package mqtt

import (
	"fmt"
	"log"

	mqtt "github.com/eclipse/paho.mqtt.golang"
)

var Message string = "0"

func GetCount() {
	client := mqtt.NewClient(mqtt.NewClientOptions().AddBroker("tcp://test.mosquitto.org:1883"))

	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error())
	}
	for {
		if token := client.Subscribe("tsc/counter", 0, func(c mqtt.Client, m mqtt.Message) {
			fmt.Printf("Message received: %s\n", m.Payload())
			Message = string(m.Payload())
		}); token.Wait() && token.Error() != nil {
			log.Fatal(token.Error())
		}
	}
}
