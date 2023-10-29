#!/usr/bin/zsh
for _ in {0..1000}

do
    # Replace the following command with the one you want to run
    # Example: echo "Hello, world!"
    # Replace it with your command
    kitty icat output.tga

    # Add a sleep to control the loop's execution speed
    sleep 0.05  # Adjust the sleep duration as needed
done
