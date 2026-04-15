# 🎨 Color Histogram & Image Quantization in C

This project implements a memory-efficient **color histogram** and a **color quantization algorithm** for PPM images using the C programming language. It was developed as part of the *Foundations of Computer Science* project at ENSICAEN.

## 📌 Features

- Efficient RGB color histogram computation
- Optimized data structure using a 2D array of linked lists
- Fast color frequency lookup
- Iterator-based traversal of histogram data
- Command-line histogram visualization tool
- Image color quantization using the **popularity algorithm**
- Pixel mapping to nearest representative colors

## 🧠 How It Works

### 🔹 Color Histogram

Instead of using a large 3D array (256³), the histogram is stored as: a **256 × 256 array**, each cell contains a **linked list of blue (B) values**, each node stores: blue value (B), frequency, and pointer to next node. This significantly reduces memory usage while maintaining efficiency.

### 🔹 Histogram Operations

The project includes: creation and initialization of the histogram from an image, insertion and update of color frequencies, memory deallocation, frequency query for any RGB triplet, and full traversal using a custom iterator.

### 🔹 Histogram CLI Tool

Run the following command: `./ppmhist image.ppm`. Output format: `(R, G, B, luminance, occurrences)` where luminance = (R + G + B) / 3.

### 🔹 Color Quantization

The project reduces an image to K dominant colors: 1) Compute the histogram, 2) Select the K most frequent colors, 3) Build a color palette, 4) Replace each pixel with the closest palette color, 5) Generate a new quantized image.

## ⚙️ Installation & Compilation

Make sure you have `gcc` installed. Then run: `gcc -o ppmhist ppmhist.c histogram.c image.c -Wall` and `gcc -o quantification quantification.c histogram.c image.c -Wall`.

## 🚀 Usage

1. Compute Histogram: `./ppmhist input.ppm`
2. Quantize an Image: `./quantification K input.ppm output.ppm` where K is the number of colors to keep, input.ppm is the original image, and output.ppm is the quantized image.

## 📁 Project Structure
## 📁 Project Structure

```bash
.
├── histogram.c
├── histogram.h
├── quantification.c
├── ppmhist.c
├── image.c
├── image.h
└── README.md
```

## 🔧 Optional Improvements

Approximate color grouping: R/2 = R'/2, G = G', B/4 = B'/4. Also faster nearest-neighbor search and improved quantization quality.

## 🎯 Learning Outcomes

Advanced data structures (linked lists, iterators), memory-efficient design, image processing basics, algorithm optimization, and practical C programming.

## 📜 License

This project is for educational purposes.
