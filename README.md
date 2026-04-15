# 🎨 Color Histogram & Image Quantization in C

This project implements a memory-efficient **color histogram** and a **color quantization algorithm** for PPM images using the C programming language. It was developed as part of the *Foundations of Computer Science* project at ENSICAEN.

---

## 📌 Features

- Efficient RGB color histogram computation  
- Optimized data structure using a 2D array of linked lists  
- Fast color frequency lookup  
- Iterator-based traversal of histogram data  
- Command-line histogram visualization tool  
- Image color quantization using the **popularity algorithm**  
- Pixel mapping to nearest representative colors  

---

## 🧠 How It Works

### 🔹 Color Histogram

Instead of using a large 3D array (256³), the histogram is stored as:

- A **256 × 256 array**
- Each cell contains a **linked list of blue (B) values**
- Each node stores:
  - Blue value (B)
  - Frequency
  - Pointer to next node

This significantly reduces memory usage while maintaining efficiency.

---

### 🔹 Histogram Operations

The project includes:

- Creation and initialization of the histogram from an image  
- Insertion and update of color frequencies  
- Memory deallocation  
- Frequency query for any RGB triplet  
- Full traversal using a custom iterator  

---

### 🔹 Histogram CLI Tool

Run the following command:

```bash
./ppmhist image.ppm
Output format:

text
(R, G, B, luminance, occurrences)
Where:

luminance = (R + G + B) / 3

🔹 Color Quantization
The project reduces an image to K dominant colors:

Compute the histogram

Select the K most frequent colors

Build a color palette

Replace each pixel with the closest palette color

Generate a new quantized image

⚙️ Installation & Compilation
Make sure you have gcc installed.

bash
gcc -o ppmhist ppmhist.c histogram.c image.c -Wall
gcc -o quantification quantification.c histogram.c image.c -Wall
🚀 Usage
1. Compute Histogram
bash
./ppmhist input.ppm
2. Quantize an Image
bash
./quantification K input.ppm output.ppm
K: number of colors to keep

input.ppm: original image

output.ppm: quantized image

📁 Project Structure
text
.
├── histogram.c        # Histogram implementation
├── histogram.h        # Histogram interface
├── quantification.c   # Quantization algorithm
├── ppmhist.c          # Histogram CLI tool
├── image.c            # Image handling (PPM)
├── image.h
└── README.md
🔧 Optional Improvements
Approximate color grouping:

R/2 = R'/2

G = G'

B/4 = B'/4

Faster nearest-neighbor search

Improved quantization quality

🎯 Learning Outcomes
Advanced data structures (linked lists, iterators)

Memory-efficient design

Image processing basics

Algorithm optimization

Practical C programming

📜 License
This project is for educational purposes.
