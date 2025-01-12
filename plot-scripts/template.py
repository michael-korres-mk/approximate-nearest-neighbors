def plot(filePath, groupby_cols, columns, labels, colors, outputTitle, csv_mean_file_path):
    import utils
    import os
    import matplotlib.pyplot as plt
    import numpy as np
    import pandas as pd

    df = utils.readData(filePath)

    output_dir = "experiment-plots/"
    os.makedirs(output_dir, exist_ok=True)

    # If groupby_cols is a single string, convert it to a list
    if isinstance(groupby_cols, str):
        groupby_cols = [groupby_cols]

    # Get all unique combinations of values for each column in groupby_cols
    unique_combinations = df[groupby_cols].drop_duplicates().sort_values(groupby_cols)
    num_combinations = len(unique_combinations)

    # Create subplots for each unique group combination
    fig, axes = plt.subplots(num_combinations, 1, figsize=(10, 6 * num_combinations))

    # If only one subplot, make axes iterable
    axes = [axes] if num_combinations == 1 else axes

    # Collect all mean data for Excel output
    mean_data_list = []

    # Iterate over each unique combination of grouping columns
    for j, group_values in enumerate(unique_combinations.itertuples(index=False)):
        group_filter = (df[groupby_cols] == pd.Series(group_values, index=groupby_cols)).all(axis=1)
        group_df = df[group_filter]

        mean_df = group_df.groupby('NUM_OF_THREADS')[columns].mean().reset_index()

        # Add group information for clarity in output
        for col, value in zip(groupby_cols, group_values):
            mean_df[col] = value

        mean_data_list.append(mean_df)

        # Prepare x-axis values and plot width
        thread_counts = mean_df['NUM_OF_THREADS']
        x = np.arange(len(thread_counts))
        width = 0.35

        ax = axes[j]

        # Plot each column's mean value as a bar
        for i, col in enumerate(columns):
            ax.bar(
                x + i * width - width / 2,
                mean_df[col],
                width,
                label=labels[col],
                color=colors[col]
            )

        # Set plot title, labels, and legend
        group_label = ", ".join(f"{col}={val}" for col, val in zip(groupby_cols, group_values))
        ax.set_xticks(x)
        ax.set_xticklabels(thread_counts)
        ax.set_xlabel('Number of Threads')
        ax.set_ylabel('Mean Time (s)')
        ax.set_title(f'Mean Execution Times - Group: {group_label}')
        ax.legend()

    # Save each group's mean data to a separate sheet in a single Excel file
    with pd.ExcelWriter(csv_mean_file_path, engine='openpyxl') as writer:
        for mean_df, group_values in zip(mean_data_list, unique_combinations.itertuples(index=False)):
            sheet_name = "_".join(str(value) for value in group_values)[:31]  # Excel sheet names are max 31 chars
            mean_df.to_excel(writer, sheet_name=sheet_name, index=False)

            # Set column widths
            column_widths = [25] + [30] * (len(df.columns) + 1)  # Custom widths for each column
            utils.set_column_widths(writer, sheet_name, column_widths)


    # Adjust layout and save the figure
    plt.tight_layout()
    output_file = os.path.join(output_dir, f"{outputTitle}.png")
    plt.savefig(output_file)

    # Close the plot to free memory
    plt.close()

    # Output the file path
    print(f"Plot saved: {output_file}")
    print(f"Mean data saved to: {csv_mean_file_path}")

def plot_no_groupby(filePath, columns, labels, colors, outputTitle, csv_mean_file_path):
    import utils
    import os
    import matplotlib.pyplot as plt
    import numpy as np
    import pandas as pd

    # Read the data
    df = utils.readData(filePath)

    # Create output directory if it doesn't exist
    output_dir = "experiment-plots/"
    os.makedirs(output_dir, exist_ok=True)

    # Calculate the mean for each 'NUM_OF_THREADS' across the entire dataset
    mean_df = df.groupby('NUM_OF_THREADS')[columns].mean().reset_index()

    # Prepare x-axis values and plot width
    thread_counts = mean_df['NUM_OF_THREADS']
    x = np.arange(len(thread_counts))
    width = 0.35

    # Create the plot
    fig, ax = plt.subplots(figsize=(10, 6))

    # Plot each column's mean value as a bar
    for i, col in enumerate(columns):
        ax.bar(
            x + i * width - width / 2,
            mean_df[col],
            width,
            label=labels[col],
            color=colors[col]
        )

    # Set plot title, labels, and legend
    ax.set_xticks(x)
    ax.set_xticklabels(thread_counts)
    ax.set_xlabel('Number of Threads')
    ax.set_ylabel('Mean Time (s)')
    ax.set_title(f'Mean Execution Times - {outputTitle}')
    ax.legend()

    # Save the plot
    plt.tight_layout()
    output_file = os.path.join(output_dir, f"{outputTitle}.png")
    plt.savefig(output_file)

    # Close the plot to free memory
    plt.close()

    # Save the mean data to an Excel file
    mean_df.to_excel(csv_mean_file_path, index=False)

    # Output the file paths
    print(f"Plot saved: {output_file}")
    print(f"Mean data saved to: {csv_mean_file_path}")
