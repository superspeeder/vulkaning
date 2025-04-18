import os
import glob

files = glob.glob("src/**.*",recursive=True)

def compile_shader(f):
    out = os.path.join("spv", os.path.relpath(f, "src/"))
    os.makedirs(os.path.dirname(out), exist_ok=True)
    os.system(f"glslc {f} -o {out}.spv")
    print(f"built {f} --> {out}")

for file in files:
    compile_shader(file)
