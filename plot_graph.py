import pandas as pd
import matplotlib.pyplot as plt

def plot_performance(csv_file, output_file="../performance_plot.png"):
    try:
        data = pd.read_csv(csv_file)

        test_sizes = list(map(int, data.columns[1:]))
        plt.figure(figsize=(10, 6))

        x_min, x_max = min(test_sizes), max(test_sizes)
        y_min, y_max = float('inf'), float('-inf')

        for _, row in data.iterrows():
            test_name = row[0]
            timings = row[1:].values

            y_min = min(y_min, *timings[~pd.isna(timings)])
            y_max = max(y_max, *timings[~pd.isna(timings)])

            valid_sizes = [size for size, timing in zip(test_sizes, timings) if not pd.isna(timing)]
            valid_timings = [timing for timing in timings if not pd.isna(timing)]

            plt.plot(valid_sizes, valid_timings, marker='o', label=test_name)

        plt.xlim(x_min, x_max)
        plt.ylim(y_min * 0.9, y_max * 1.1)

        plt.title("Structs Testing", fontsize=14)
        plt.xlabel("Test Size", fontsize=12)
        plt.ylabel("Test Time (ms)", fontsize=12)
        plt.legend(title="Test Name", fontsize=10)
        plt.grid(True, linestyle='--', alpha=0.6)

        plt.savefig(output_file)
        plt.show()

        print(f"Graph saved to {output_file}")
    except Exception as e:
        print(f"An error occurred: {e}")

csv_file = 'performance_data.csv'
plot_performance(csv_file)