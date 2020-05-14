from PIL import Image, ImageOps
import fileinput



## flip a sprite file
# im = Image.open("C:/Users/bbari/source/repos/cqlmdnk/GHC/resources/dem/fire/FireSprites.png")
# imageS = Image.new("RGB", (im.width,im.height))
# dst = Image.new("RGB", (int(im.width/4),im.height))
# for i in range(4):
#     dst = im.crop(((int(im.width / 4)) * i, 0, (int(im.width / 4)) * (i + 1),
#                     im.height))
#     dst.save("C:/Users/bbari/source/repos/cqlmdnk/GHC/resources/dem/fire/FireSprites"+str(i)+".png")
#     dst = ImageOps.mirror(dst)
#     imageS.paste(dst, (int((im.width / 4) * i), 0))
# imageS.save("C:/Users/bbari/source/repos/cqlmdnk/GHC/resources/demon_attack_s.bmp")




##pixel purple corrector

import glob

# c = 0
# dst = Image.new("RGB", (768,128))
# dst_l = Image.new("RGB", (768,128))

# for filename in glob.glob("*.png"):
#     im = Image.open(filename)
    
#     dst_l.paste(im, (int(c*96), 0))
#     # dst = im.crop(((int(im.width / 4)) * i, 0, (int(im.width / 4)) * (i + 1),
#     #                  im.height))
# #     dst.save("C:/Users/bbari/source/repos/cqlmdnk/GHC/resources/dem/fire/FireSprites"+str(i)+".png")
#     im = ImageOps.mirror(im)
#     dst.paste(im, (int(c*96), 0))
#     c+=1
# dst_l.save("s.bmp")

# dst.save("m.bmp")  
    
for filename in glob.glob("*.bmp"):
    im = Image.open(filename)
    pixelMap = im.load()
    
    img = Image.new( im.mode, im.size)
    pixelsNew = img.load()
    for i in range(img.size[0]):
        for j in range(img.size[1]):
            if pixelMap[i,j][0] >180 and pixelMap[i,j][1] <60 and pixelMap[i,j][2] >180:
                  pixelsNew[i,j] = (255,0,255)
            else:
                pixelsNew[i,j] = pixelMap[i,j]
    img = img.convert("P")            
    img.save(filename)

