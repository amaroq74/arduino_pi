
import paho.mqtt.client as mqtt

CmndTopics = [ ['cmnd/sonoff1/out1'],
               ['cmnd/sonoff2/out1'],
               ['cmnd/sonoff3/out1'],
               ['cmnd/sonoff4/out1'],
               ['cmnd/sonoff5/out1','cmnd/sonoff5/out2', 'cmnd/sonoff5/out3','cmnd/sonoff5/out4'],
               ['cmnd/sonoff6/out1','cmnd/sonoff6/out2', 'cmnd/sonoff6/out3','cmnd/sonoff6/out4'],
               ['cmnd/sonoff7/out1','cmnd/sonoff7/out2', 'cmnd/sonoff7/out3','cmnd/sonoff7/out4'] ]

StatTopics = [ ['stat/sonoff1/out1'],
               ['stat/sonoff2/out1'],
               ['stat/sonoff3/out1'],
               ['stat/sonoff4/out1'],
               ['stat/sonoff5/out1','stat/sonoff5/out2', 'stat/sonoff5/out3','stat/sonoff5/out4'],
               ['stat/sonoff6/out1','stat/sonoff6/out2', 'stat/sonoff6/out3','stat/sonoff6/out4'],
               ['stat/sonoff7/out1','stat/sonoff7/out2', 'stat/sonoff7/out3','stat/sonoff7/out4'] ]


def on_connect(client, userdata, flags, rc):
    for dev in StatTopics:
        for chan in dev:
            client.subscribe(chan)

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("aliska.amaroq.net", 1883, 60)

client.loop_start()

def setOn(dev, chan):
    client.publish(CmndTopics[dev][chan], "ON")

def setOff(dev, chan):
    client.publish(CmndTopics[dev][chan], "OFF")

