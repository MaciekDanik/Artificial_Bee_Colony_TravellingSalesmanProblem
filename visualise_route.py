import matplotlib.pyplot as plt

# Funkcja do wczytywania danych z pliku
def load_results(filename):
    cities = {}
    best_route = []
    total_distance = 0
    
    with open(filename, "r", encoding="utf-8") as file:  # Dodajemy obsługę UTF-8
        lines = file.readlines()
        reading_cities = False
        reading_route = False
        
        for i in range(len(lines)):
            line = lines[i].strip()
            if not line:  # Ignorowanie pustych linii
                continue
            
            if line.startswith("Cities:"):
                reading_cities = True
                reading_route = False
                continue
            elif line.startswith("Best Route:"):
                reading_cities = False
                reading_route = True
                continue
            elif line.startswith("Total Distance:"):
                reading_cities = False
                reading_route = False
                # Pobranie następnej linii jako dystansu
                if i + 1 < len(lines):
                    try:
                        total_distance = float(lines[i + 1].strip())  # Parsowanie liczby
                    except ValueError:
                        print("Unable to read Total Distance!")
                        total_distance = 0
                continue
                
            if reading_cities:
                parts = line.split()
                if len(parts) < 3:  # Sprawdzamy, czy linia ma odpowiednią ilość danych
                    print(f"Wrong line in section Cities: {line}")
                    continue
                
                city_id = int(parts[0])
                x, y = float(parts[1]), float(parts[2])
                cities[city_id] = (x, y)
            elif reading_route:
                best_route = list(map(int, line.split()))
                
    return cities, best_route, total_distance

# Funkcja do wizualizacji trasy
def plot_route(cities, best_route, total_distance, title):
    plt.figure(figsize=(8, 6))
    
    # Rysowanie miast
    for city_id, (x, y) in cities.items():
        plt.scatter(x, y, color='blue', s=100)
        plt.text(x, y, str(city_id), fontsize=15, ha='right', color='red')

    # Rysowanie połączeń między miastami
    for i in range(len(best_route) - 1):
        c1, c2 = best_route[i], best_route[i + 1]
        x_values = [cities[c1][0], cities[c2][0]]
        y_values = [cities[c1][1], cities[c2][1]]
        plt.plot(x_values, y_values, 'k-', lw=2)

    c1, c2 = best_route[-1], best_route[0]
    x_values = [cities[c1][0], cities[c2][0]]
    y_values = [cities[c1][1], cities[c2][1]]
    plt.plot(x_values, y_values, 'k-', lw=2)
    
    # Dodanie tytułu
    plt.title(f"Best Track with {title}| Distance: {total_distance:.2f}")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.grid(True)
    plt.show()

def plot_route(cities, best_route, total_distance, title):
    plt.figure(figsize=(8, 6))
    
    # Rysowanie miast
    for city_id, (x, y) in cities.items():
        plt.scatter(x, y, color='blue', s=100)
        plt.text(x, y, str(city_id), fontsize=15, ha='right', color='red')

    # Rysowanie połączeń między miastami
    for i in range(len(best_route) - 1):
        c1, c2 = best_route[i], best_route[i + 1]
        x_values = [cities[c1][0], cities[c2][0]]
        y_values = [cities[c1][1], cities[c2][1]]
        plt.plot(x_values, y_values, 'k-', lw=2)

    c1, c2 = best_route[-1], best_route[0]
    x_values = [cities[c1][0], cities[c2][0]]
    y_values = [cities[c1][1], cities[c2][1]]
    plt.plot(x_values, y_values, 'k-', lw=2)
    
    # Dodanie tytułu
    plt.title(f"Best Track with {title}| Distance: {total_distance:.2f}")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.grid(True)
    plt.show()


def plot_both_routes(cities, bee_best_route, nn_best_route, bee_total_distance, nn_total_distance, bee_title, nn_title):
    plt.figure(figsize=(10, 8))
    
    # Rysowanie miast
    for city_id, (x, y) in cities.items():
        plt.scatter(x, y, color='green', s=100)
        plt.text(x, y, str(city_id), fontsize=12, ha='right', va='bottom', color='black')
    
    # Rysowanie trasy Bee
    bee_x = [cities[city][0] for city in bee_best_route] + [cities[bee_best_route[0]][0]]
    bee_y = [cities[city][1] for city in bee_best_route] + [cities[bee_best_route[0]][1]]
    plt.plot(bee_x, bee_y, 'r-', lw=2, label=f'{bee_title} (Distance: {bee_total_distance:.2f})')
    
    # Rysowanie trasy NN
    nn_x = [cities[city][0] for city in nn_best_route] + [cities[nn_best_route[0]][0]]
    nn_y = [cities[city][1] for city in nn_best_route] + [cities[nn_best_route[0]][1]]
    plt.plot(nn_x, nn_y, 'b--', lw=2, label=f'{nn_title} (Distance: {nn_total_distance:.2f})')

    # Ustawienia wykresu
    plt.title("Comparison of Two Routes")
    plt.xlabel("X")
    plt.ylabel("Y20")
    plt.legend()
    plt.grid(True)
    plt.show()

# Wczytanie danych z pliku -- algorytm pszczeli
#bee_file = "results_bee_multi.txt"
#bee_file = "results_bee_0.txt"
#cities, bee_best_route, bee_total_distance = load_results(bee_file)

# CITIES = [10, 25, 50, 100]#, 250, 500, 1000] #cities.size()
CITIES = [200]

#zaczytanie danych z różnych plików
cities_list = list()
bee_best_route = list()
bee_total_distance = list()

for i in CITIES:
    bee_file = "Bee_results/results_bee_" + str(i) + "_cities.txt"
    _cities, _bee_best_route, _bee_total_distance = load_results(bee_file)
    cities_list.append(_cities)
    bee_best_route.append(_bee_best_route)
    bee_total_distance.append(_bee_total_distance)

# Wczytanie danych z pliku -- algorytm najbliższego sąsiada
# nn_file = "results_nn_multi.txt"
# nn_cities, nn_best_route, nn_total_distance = load_results(nn_file)

cities_list_nn = list()
nn_best_route = list()
nn_total_distance = list()

for i in CITIES:
    nn_file = "NN_results/results_nn_" + str(i) + "_cities.txt"
    _cities, _nn_best_route, _nn_total_distance = load_results(nn_file)
    cities_list_nn.append(_cities)
    nn_best_route.append(_nn_best_route)
    nn_total_distance.append(_nn_total_distance)

# print(cities_list)
# print(bee_best_route)
# print(bee_total_distance)

# Wizualizacja trasy
#plot_route(cities_list[0], bee_best_route[0], bee_total_distance[0], "ABC alg")
#plot_route(cities, nn_best_route, nn_total_distance, "NN alg")

i=0
for _ in CITIES:
    #plt.ion()
    plot_route(cities_list[i], bee_best_route[i], bee_total_distance[i], "ABC alg")
    plot_route(cities_list_nn[i], nn_best_route[i], nn_total_distance[i], "NN alg")
    # plot_both_routes(cities_list[i], bee_best_route[i], nn_best_route[i], bee_total_distance[i], nn_total_distance[i], "ABC alg", "NN alg")
    #input("Press 'Enter' to close diagrams...")
    #plt.ioff()
    i += 1


