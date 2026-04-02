import joblib
import m2cgen as m2c

model = joblib.load("decision_tree_model.pkl")

code = m2c.export_to_c(model)

with open("model.c", "w") as f:
    f.write(code)

print("✅ C code generated → model.c")