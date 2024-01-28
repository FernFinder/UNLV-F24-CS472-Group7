import csv
import matplotlib.pyplot as plt
from datetime import datetime

# Function to convert ISO 8601 date format to week number
def iso8601_to_week(iso_date):
    dt = datetime.fromisoformat(iso_date.replace("Z", "+00:00"))
    return dt.strftime('%G-W%V')

# Function to generate scatter plot
def generate_scatter_plot(authors_file, files_file):
    # Read authors and dates from CSV
    authors_data = []
    with open(authors_file, 'r') as file:
        reader = csv.reader(file)
        header = next(reader)
        for row in reader:
            authors_data.append(row)

    # Read files and touches from CSV
    files_data = {}
    with open(files_file, 'r') as file:
        reader = csv.reader(file)
        header = next(reader)
        for row in reader:
            files_data[row[0]] = int(row[1])

    # Extract distinct authors
    authors_set = set([row[1] for row in authors_data])

    # Create a color map for each author
    color_map = {author: plt.cm.tab10(i) for i, author in enumerate(authors_set)}

    # Create scatter plot
    for author in authors_set:
        x_values = []  # Weeks
        y_values = []  # File variables
        c_values = []  # Color (author)

        for row in authors_data:
            if row[1] == author:
                filename = row[0]
                if filename in files_data:
                    touches = files_data[filename]
                    date = row[2]
                    week = iso8601_to_week(date)
                    x_values.append(week)
                    y_values.append(touches)
                    c_values.append(color_map[author])

        plt.scatter(x_values, y_values, label=author, c=c_values, alpha=0.7)

    # Configure plot
    plt.xlabel('Weeks')
    plt.ylabel('File Variables')
    plt.title('Scatter Plot of Weeks vs File Variables (Shaded by Author)')
    plt.legend()
    plt.xticks(rotation=45)
    plt.grid(True)

    # Show the plot
    plt.show()

# Input CSV files
authors_file_input = 'data/authors_rootbeer.csv'
files_file_input = 'data/file_rootbeer.csv'

# Generate scatter plot
generate_scatter_plot(authors_file_input, files_file_input)

# Wait for user input before closing the plot
input("Press Enter to close the plot.")

