!/bin/zsh
for _ in {0..1000}

do
    # Replace the following command with the one you want to run
    # Example: echo "Hello, world!"
    # Replace it with your command
    magick convert output.tga output.png
    kitty icat output.png

    # Add a sleep to control the loop's execution speed
    sleep 0.05  # Adjust the sleep duration as needed
done
