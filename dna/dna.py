import csv
import sys


def longest_match(sequence, subsequence):
    """Return length of longest run of consecutive repeats of subsequence in sequence."""
    max_count = 0
    current_count = 0
    i = 0

    while i < len(sequence):
        # Check for the STR
        if sequence[i:i + len(subsequence)] == subsequence:
            current_count += 1
            i += len(subsequence)
        else:
            max_count = max(max_count, current_count)
            current_count = 0
            i += 1

    return max(max_count, current_count)  # Final check


def load_database(filename):
    """Load the DNA database from a CSV file."""
    database = []
    with open(filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            database.append(row)
    return database


def load_dna_sequence(filename):
    """Load the DNA sequence from a text file."""
    with open(filename, 'r') as file:
        return file.read().strip()


def main():
    # Check for the correct number of command-line arguments
    if len(sys.argv) != 3:
        print("Usage: python dna_identifier.py database.csv sequence.txt")
        return

    # Load the DNA database and DNA sequence
    database = load_database(sys.argv[1])
    dna_sequence = load_dna_sequence(sys.argv[2])

    # Get the STRs from the database (excluding the name column)
    strs = list(database[0].keys())[1:]

    # Count the longest runs of each STR in the DNA sequence
    str_counts = {str_seq: longest_match(dna_sequence, str_seq) for str_seq in strs}

    # Identify the person based on STR counts
    for person in database:
        match = True
        for str_seq in strs:
            if int(person[str_seq]) != str_counts[str_seq]:
                match = False
                break
        if match:
            print(person['name'])
            return

    print("No match")


if __name__ == "__main__":
    main()
