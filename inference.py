import numpy as np
import joblib

model = joblib.load("fall_detection_knn_model.pkl")
scaler = joblib.load("scaler.pkl")

sample_data = np.array([[12.78,1.01,6.89,3.02,114.22,56.32,7,2,5.8]])

sample_data_scaled = scaler.transform(sample_data)

prediction = model.predict(sample_data_scaled)
probability = model.predict_proba(sample_data_scaled)

print("===== INFERENCE RESULT =====")
print(f"Predicted Class: {prediction[0]}")
print(f"Confidence     : {np.max(probability) * 100:.2f}%")

if prediction[0] == 1:
    print("Status         : Fall Detected!")
else:
    print("Status         : No Fall.")