def main():

    import utils
    for i in ['filterann','ann']:
        csv_file = f"experiment-results/{i}.csv"
        excel_file = f'experiment-results/{i}.xlsx'
        utils.csv_to_excel(csv_file, excel_file)

# Entry point of the program
if __name__ == "__main__":
    main()