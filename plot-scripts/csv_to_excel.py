def main():

    import utils
    for filename in ['filterann','ann']:
        csv_file = f"experiment-results/{filename}.csv"
        excel_file = f'experiment-results/{filename}.xlsx'
        utils.csv_to_excel(csv_file, excel_file)

# Entry point of the program
if __name__ == "__main__":
    main()