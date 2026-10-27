python
import os

# Define the project root directory name
root_dir = "CarromPredictor"

# List of all directories that need to be created
directories = [
    os.path.join(root_dir, ".github", "workflows"),
    os.path.join(root_dir, "java", "com", "predictor"),
    os.path.join(root_dir, "jni")
]

# List of all empty files to initialize inside the structure
files = [
    os.path.join(root_dir, ".github", "workflows", "build.yml"),
    os.path.join(root_dir, "AndroidManifest.xml"),
    os.path.join(root_dir, "java", "com", "predictor", "MainActivity.java"),
    os.path.join(root_dir, "java", "com", "predictor", "OverlayService.java"),
    os.path.join(root_dir, "jni", "Android.mk"),
    os.path.join(root_dir, "jni", "Application.mk"),
    os.path.join(root_dir, "jni", "memread.cpp")
]

print("🚀 Starting folder generation setup...")

# Step 1: Create all subdirectories safely
for directory in directories:
    os.makedirs(directory, exist_ok=True)
    print(f"📁 Created folder path: {directory}")

# Step 2: Initialize empty text files in their correct folders
for file_path in files:
    with open(file_path, "w", encoding="utf-8") as f:
        pass  # Just create/clear the file without adding content yet
    print(f"📄 Created empty file: {file_path}")

print("\n✅ Success! Your workspace structure is completely built and ready.")