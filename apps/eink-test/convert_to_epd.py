from PIL import Image
image_file = Image.open('lab11_eink_color.png') # open colour image
image_file = image_file.convert('1') # convert image to black and white
image_file.save('result.png')

pixels = image_file.load()

# print(len(pixels))
print(image_file.size)

out = []

for y in range(0, 300):
	for x in range(0, 400):
		if x < image_file.size[0] and y < image_file.size[1]:
			if pixels[x, y] == 255:
				out.append(0)
			else:
				out.append(1)
		else:
			out.append(0)

# print(out)

compressed = []

# compress to buts
for i in range(0, 300*400, 8):
	val = out[i]   * 0x80 + \
	      out[i+1] * 0x40 + \
	      out[i+2] * 0x20 + \
	      out[i+3] * 0x10 + \
	      out[i+4] * 0x08 + \
	      out[i+5] * 0x04 + \
	      out[i+6] * 0x02 + \
	      out[i+7] * 0x01

	compressed.append(val)

# print(compressed)
# print(len(compressed))


def chunks(l, n):
	"""Yield successive n-sized chunks from l."""
	for i in range(0, len(l), n):
		yield l[i:i+n]


print('uint8_t img[15000] = {')
for c in chunks(compressed, 50):
	# print(c)
	a = ','.join(map(str, c))
	print(a + ',')
	# print('{},'.format(','.join(c)))

print('};')