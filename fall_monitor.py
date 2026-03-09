import serial
import requests

SERIAL_PORT = "COM8"   # change if needed
TOPIC = "fall-alert"

ser = serial.Serial(SERIAL_PORT, 115200)

print("Monitoring serial...")

while True:
    line = ser.readline().decode().strip()
    print(line)

    if "FALL DETECTED" in line:
        print("Sending notification...")

        requests.post(
            f"https://ntfy.sh/{TOPIC}",
            data="⚠ Fall detected!".encode("utf-8")
        )