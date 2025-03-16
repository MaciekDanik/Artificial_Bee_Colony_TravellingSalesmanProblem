import matplotlib.pyplot as plt

# Funkcja do wczytywania danych z pliku
def load_results(filename="results.txt"):
    cities = {}
    best_route = []
    total_distance = 0
    
    with open(filename, "r", encoding="utf-8") as file:  # Dodajemy obsługę UTF-8
        lines = file.readlines()
        reading_cities = False
        reading_route = False
        
        for line in lines:
            line = line.strip()
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
                try:
                    total_distance = float(line.split(":")[1].strip())  # Poprawne parsowanie liczby
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
def plot_route(cities, best_route, total_distance):
    plt.figure(figsize=(8, 6))
    
    # Rysowanie miast
    for city_id, (x, y) in cities.items():
        plt.scatter(x, y, color='blue', s=100)
        plt.text(x, y, str(city_id), fontsize=12, ha='right', color='red')
    
    # Rysowanie połączeń między miastami
    for i in range(len(best_route) - 1):
        c1, c2 = best_route[i], best_route[i + 1]
        x_values = [cities[c1][0], cities[c2][0]]
        y_values = [cities[c1][1], cities[c2][1]]
        plt.plot(x_values, y_values, 'k-', lw=2)
    
    # Dodanie tytułu
    plt.title(f"Best Track | Distance: {total_distance:.2f}")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.grid(True)
    plt.show()

# Wczytanie danych z pliku
data_file = "results.txt"
cities, best_route, total_distance = load_results(data_file)

# Wizualizacja trasy
plot_route(cities, best_route, total_distance)
