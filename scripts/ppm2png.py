import os, sys, glob
import numpy as np
from PIL import Image

if __name__=='__main__':

	files = []

	input_files = '*.ppm'
	output_folder = ''

	if len(sys.argv) > 1:
		input_files = sys.argv[1]
	if len(sys.argv) > 2:
		output_folder = sys.argv[2]
	
	try:
		os.mkdir(output_folder)
	except:
		pass

	try:
		for file in glob.glob(input_files):
			if file.split('.')[-1] == 'ppm':
				files.append(os.path.normpath(file))
	except Exception as e:
		print(e)
	if len(files) == 0:
		print('Nenhum arquivo .ppm encontrado')
	else:
		print(str(len(files))+' arquivos'if len(files) > 1 else 'arquivo'+' encontrados')

	for file in files:
		print('Convertendo arquivo '+os.path.basename(file))
		with open(file) as f:
			f.readline()
			width, height = [int(i) for i in f.readline().split(' ')]
			
			img_buffer = np.zeros((height, width, 3), dtype=np.uint8)
			max = int(f.readline())
			index = 0
			for line in f.readlines():
				try:
					r, g, b = [int(i) for i in line.split(' ')]
				except ValueError as e:
					print(e)
					pass
				i, j = index//width, index % width
				img_buffer[i, j, :] = (r, g, b)
				index += 1
			img = Image.fromarray(img_buffer, 'RGB')
			img.save(os.path.join(output_folder,''.join(os.path.basename(file).split('.')[:-1])+'.png'))
