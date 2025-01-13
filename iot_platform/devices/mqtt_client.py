import paho.mqtt.client as mqtt
from .models import Device, DeviceMessage

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker with result code " + str(rc))
    client.subscribe("iot/devices/#")

def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode('utf-8')

    device_name = topic.split("/")[-1]
    device, created = Device.objects.get_or_create(name=device_name)
    device.last_message = payload
    device.status = True
    device.save()
    DeviceMessage.objects.create(
        device=device,
        message=payload
    )

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("localhost", 1883, 60)
