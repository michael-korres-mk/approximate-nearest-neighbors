def main():
    groupby = []
    filePath = "experiment-results/ann.csv"
    columns = ['SERIAL_ALGORITHM_TIME', 'PARALLEL_ALGORITHM_TIME']
    labels = dict(zip(columns, ['Serial', 'Parallel']))  # Mapping columns to labels
    colors = dict(zip(columns, ['blue', 'orange']))  # Mapping columns to colors
    outputTitle = "ann_mean_execution_times"
    csv_mean_file_path = "experiment-results/ann_mean.xlsx"

    template.plot(filePath, groupby, columns, labels, colors,outputTitle,csv_mean_file_path)

# Entry point of the program
if __name__ == "__main__":
    import template
    main()