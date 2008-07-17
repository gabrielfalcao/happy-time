from ctypes import CDLL
svg2pdf = CDLL("./libsvg2pdf.so")
svg2pdf.svg2pdf("report.svg", "teste2.pdf")
svg2pdf.svg2pdf2("report.svg", "teste3.pdf")
