import serial
import pandas as pd
import time

ser = serial.Serial('COM3', 115200)

data = []

print("Waiting for Trigger 1 (Free Fall)...")

recording = False

while True:
    line = ser.readline().decode(errors='ignore').strip()

    # Start recording when trigger detected
    if "START_RECORDING" in line:
        print("Trigger 1 detected → Recording started")
        recording = True
        start_time = time.time()
        data = []
        continue

    if recording:
        try:
            values = list(map(float, line.split(",")))

            if len(values) == 8:
                data.append(values)

        except:
            continue

        # Record for 5 seconds
        if time.time() - start_time >= 5:
            print("Recording complete")

            columns = ["ax","ay","az","gx","gy","gz","Amp","Angle"]
            df = pd.DataFrame(data, columns=columns)

            label = int(input("Enter label (1 = fall, 0 = non-fall): "))
            df["label"] = label

            filename = f"dataset_{int(time.time())}.csv"
            df.to_csv(filename, index=False)

            print(f"Saved: {filename}\n")

            recording = False
