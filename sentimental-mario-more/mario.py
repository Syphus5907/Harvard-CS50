def get_int(prompt):
    while True:
        try:
            value = int(input(prompt))
            if 1 <= value <= 8:
                return value
            else:
                print("Please enter a positive integer between 1 and 8.")
        except ValueError:
            print("Invalid input. Please enter an integer.")


def half_pyramids(height):
    for i in range(1, height + 1):
        # Print left pyramid
        left_pyramid = ' ' * (height - i) + '#' * i
        # Print right pyramid
        right_pyramid = ' ' * 2 + '#' * i
        print(left_pyramid + right_pyramid)


# Prompt the user for the height of the half-pyramids
height = get_int("Enter the height of the half-pyramids (1-8): ")

# Print the half-pyramids
half_pyramids(height)
