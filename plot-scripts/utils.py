import os
import pandas as pd
from openpyxl.utils import get_column_letter


def readData(filePath):
    # Check if file exists
    if os.path.exists(filePath) == False:
        print(f"File not found: {filePath}")
        return None
    try:
        # Try reading the file
        return pd.read_csv(filePath, sep=',')
    except Exception as e:
        # Handle errors
        print(f"Error reading the file: {e}")
        return None


def csv_to_excel(csv_file, excel_file):
    try:
        # Read CSV file
        df = pd.read_csv(csv_file)

        # Save as Excel file
        df.to_excel(excel_file, index=False, engine='openpyxl')

        sheet_name = "main"
        # Save each group's mean data to a separate sheet in a single Excel file
        with pd.ExcelWriter(excel_file, engine='openpyxl') as writer:
            df.to_excel(writer, sheet_name=sheet_name, index=False)

            # Set column widths
            column_widths = [25] + [30] * (len(df.columns) + 1)  # Custom widths for each column
            set_column_widths(writer, sheet_name, column_widths)

        print(f"Successfully converted '{csv_file}' to '{excel_file}'")
    except Exception as e:
        print(f"Error: {e}")


def set_column_widths(writer, sheet_name, widths):
    """Set column widths for a given sheet."""
    workbook = writer.book
    worksheet = writer.sheets[sheet_name]
    for col_num, width in enumerate(widths, 1):  # 1-indexed for Excel columns
        column_letter = get_column_letter(col_num)
        worksheet.column_dimensions[column_letter].width = width
