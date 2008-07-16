from ctypes import CDLL
svg2pdf = CDLL("/home/gabriel/programming/ansi_c/happy_time/cairo/svg2pdf/libsvg2pdf.so")
svg2pdf.svg2pdf("/home/gabriel/Imagens/svg/pythonisto.svg", "teste2.pdf")
svg2pdf.svg2pdf2("/home/gabriel/Imagens/svg/pythonisto.svg", "teste3.pdf")
