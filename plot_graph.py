import sys
import pandas as pd
import matplotlib.pyplot as plt

def plot_graph(file_path):
    try:
        # Чтение данных из CSV
        data = pd.read_csv(file_path, header=None, names=["X", "Y"])
        x = data["X"]
        y = data["Y"]

        # Построение графика
        plt.figure(figsize=(10, 6))
        plt.plot(x, y, marker='o', label='Data Points')
        plt.title("Performance Graph")
        plt.xlabel("X")
        plt.ylabel("Y")
        plt.legend()
        plt.grid()
        plt.show()
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 plot_graph.py <path_to_csv>")
    else:
        plot_graph(sys.argv[1])