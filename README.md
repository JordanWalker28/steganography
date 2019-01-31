# steganography

Small project created for one of my University courseworks. The program takes in a PPM image and offers a prompt in the terminal that allows for the user to enter a hidden message that can then be decrypted with the same program by making a comparison of the pixel values held in the image to the newly generated image. 

To run the program typical gcc -o steg2 steg2.c

To Encrypt a PPM image run ./steg2 e "image name".ppm > "new image name".ppm

Here you can enter the message you wish to encrypt! 

To Decrypt the hidden message encrypted in the new image run ./steg2 d "original image".ppm "new image".ppm

