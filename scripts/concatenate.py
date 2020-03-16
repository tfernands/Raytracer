import os, sys, glob
import numpy as np
from PIL import Image

def load_ppm(ppm_path):
	with open(ppm_path) as ppm_file:
		ppm_file.readline()
		width, height = [int(i) for i in ppm_file.readline().split(' ')]
		
		img_buffer = np.zeros((height, width, 3), dtype=np.uint8)
		max = int(ppm_file.readline())
		index = 0
		for line in ppm_file.readlines():
			try:
				r, g, b = [int(i) for i in line.split(' ')]
			except ValueError as e:
				print(e)
				pass
			i, j = index//width, index % width
			img_buffer[i, j, :] = (r, g, b)
			index += 1
		return img_buffer

if __name__=='__main__':

	files = []

	input_files = '../output/*.ppm'
	output_folder = '../output/images/'

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

	#CONCATENATE
	meta = files[0].split('_')
	w = int(meta[1])
	h = int(meta[2])
	img_meta= []
	for file_path in files:
		img_meta.append({'buffer':load_ppm(file_path), 'meta':files[0].split('_')})

	img_buffer = np.zeros((h, w, 3), dtype=np.uint8)
	for img in img_meta:
		print(img['meta'])
		xs,xf,ys,yf = img['meta'][3:8]
		img_buffer[int(xs):int(xf), int(ys),int(yf)] = img['buffer']
	img = Image.fromarray(img_buffer, 'RGB')
	img.save(os.path.join(output_folder,''.join(os.path.basename(files[0]).split('.')[:-1])+'.png'))