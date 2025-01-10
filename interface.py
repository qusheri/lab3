import sys
import numpy as np
import matplotlib.pyplot as plt

def display_menu():
    print("Select Structure:")
    print("1. Sparse Vector")
    print("2. Sparse Matrix")
    structure_choice = int(input())

    print("Select Dictionary:")
    print("1. HashTable")
    print("2. BTree")
    dictionary_choice = int(input())

    return structure_choice, dictionary_choice

def plot_graph():
    x = np.linspace(0, 10, 100)
    y = np.sin(x)

    plt.figure()
    plt.plot(x, y, label="sin(x)")
    plt.title("Example Graph")
    plt.xlabel("X Axis")
    plt.ylabel("Y Axis")
    plt.legend()
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "menu":
        structure, dictionary = display_menu()
        print(f"{structure},{dictionary}")  # Output for C++
    elif len(sys.argv) > 1 and sys.argv[1] == "graph":
        plot_graph()