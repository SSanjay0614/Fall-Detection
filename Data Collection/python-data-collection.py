import serial
import pandas as pd
import time
import numpy as np

ser = serial.Serial('COM8', 115200)

print("Waiting for Trigger 1 (Free Fall)...")

recording = False

while True:
    line = ser.readline().decode(errors='ignore').strip()

    # Start recording
    if "START_RECORDING" in line:
        print("Trigger 1 detected → Recording started")
        recording = True
        start_time = time.time()

        amp_list = []
        angle_list = []

        continue

    if recording:
        try:
            values = list(map(float, line.split(",")))

            if len(values) == 8:
                Amp = values[6]
                Angle = values[7]

                amp_list.append(Amp)
                angle_list.append(Angle)

        except:
            continue

        # 5 sec window
        if time.time() - start_time >= 5:
            print("Recording complete → Extracting features")

            amp_arr = np.array(amp_list)
            angle_arr = np.array(angle_list)

            # FEATURE EXTRACTION
            max_amp = np.max(amp_arr)
            min_amp = np.min(amp_arr)
            mean_amp = np.mean(amp_arr)
            std_amp = np.std(amp_arr)

            max_angle = np.max(angle_arr)
            mean_angle = np.mean(angle_arr)

            # durations (in samples)
            freefall_duration = np.sum(amp_arr < 0.4)
            impact_duration = np.sum(amp_arr > 1.2)

            final_angle = angle_arr[-1]
            stable_angle_mean = np.mean(angle_arr[-10:])  

            
            row = {
                "max_amp": round(max_amp,2),
                "min_amp": round(min_amp,2),
                "mean_amp": round(mean_amp,2),
                "std_amp": round(std_amp,2),
                "max_angle": round(max_angle,2),
                "mean_angle": round(mean_angle,2),
                "freefall_duration": int(freefall_duration),
                "impact_duration": int(impact_duration),
                "final_angle": round(final_angle,2)
            }

            label = int(input("Enter label (1 = fall, 0 = non-fall): "))
            row["label"] = label

            df = pd.DataFrame([row])

            filename = "dataset_features.csv"

            # append mode
            try:
                old = pd.read_csv(filename)
                df = pd.concat([old, df], ignore_index=True)
            except:
                pass

            df.to_csv(filename, index=False)

            print("Saved feature row\n")

            recording = False
