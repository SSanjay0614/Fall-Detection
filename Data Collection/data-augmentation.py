import pandas as pd
import numpy as np


df = pd.read_csv("dataset.csv")

augmented_data = []

def augment_row(row):
    new_row = row.copy()

    # Add noise to continuous features
    noise_cols = [
        "max_amp","min_amp","mean_amp","std_amp",
        "max_angle","mean_angle","final_angle","stable_angle_mean"
    ]

    for col in noise_cols:
        new_row[col] = round(row[col] + np.random.normal(0, 0.5), 2)

    # Slight variation in durations
    new_row["freefall_duration"] = max(0, int(row["freefall_duration"] + np.random.choice([-1,0,1])))
    new_row["impact_duration"] = max(0, int(row["impact_duration"] + np.random.choice([-1,0,1])))

    return new_row

# Generate new samples
for i in range(5):  # 5x expansion 
    for _, row in df.iterrows():
        augmented_data.append(augment_row(row))

# Convert to DataFrame
aug_df = pd.DataFrame(augmented_data)

# Save
aug_df.to_csv("augmented_dataset.csv", index=False)

print("Augmented dataset saved!")
