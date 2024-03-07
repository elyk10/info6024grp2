import os
import subprocess

textureDirectoryName = "\\assets\\textures"

texturePath = os.getcwd() + textureDirectoryName + "\\"
print(texturePath)

# load the names of the textures files into a list
filesInDir = os.listdir(texturePath)


processedImagesList = []

# create the file if this is the first run, otherwise open in a load the names of already processed files into a list
if not os.path.exists("processedImages.txt"):
    processedImagesFile = open("processedImages.txt", 'w') 
else:

    processedImagesFile = open("processedImages.txt", 'r+')
    for line in processedImagesFile:
        processedImagesList.append(line.strip())


for f in filesInDir:
    if f.endswith('.png'):
        # check if this is a file created by this script in previous build
        if f.endswith('_Opti.png') or f.endswith('fs8.png'):
            continue
        else:
            # ensure this file hasn't already been processed
            if(f in processedImagesList): continue

            splitName = f.split('.')

            print("Processing: " + texturePath + f)
            subprocess.run("pngquant -- " + texturePath + f)
            subprocess.run("optipng -o 3 " + texturePath + f + " -out " + texturePath + splitName[0] + "_Opti.png")
            subprocess.run("PVRTexToolCLI -i " + texturePath + f)
            
            # store the file name so we know not to process it again
            processedImagesFile.write(f + "\n")

processedImagesFile.close()



