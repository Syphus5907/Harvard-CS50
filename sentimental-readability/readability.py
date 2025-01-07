from cs50 import get_string


def calculate_readability(text):
    # Initialize counters for letters, words, and sentences
    letters = sum(c.isalpha() for c in text)  # Count letters
    words = len(text.split())  # Count words
    sentences = sum(c in '.!?' for c in text)  # Count sentences

    # Calculate averages
    L = (letters / words) * 100 if words > 0 else 0
    S = (sentences / words) * 100 if words > 0 else 0

    # Calculate Coleman-Liau index
    index = 0.0588 * L - 0.296 * S - 15.8
    return round(index)


def get_grade_level(index):
    if index < 1:
        return "Before Grade 1"
    elif index >= 16:
        return "Grade 16+"
    else:
        return f"Grade {index}"


# Get user input
text = get_string("Text: ")

# Calculate readability index
grade_index = calculate_readability(text)

# Get grade level message
grade_level = get_grade_level(grade_index)

# Output the grade level
print(grade_level)
