import random
import matplotlib.pyplot as plt

def uniform_random(a, b):
    return a + random.uniform(0,1) * (b - a)

def random_array(n):
    a = [uniform_random(0,1) for _ in range(n)]
    return a

def train_test_split(population, m):
    n = len(population)
    
    train_population = []
    test_population = []

    train_population = random.sample(population, m)

    for i in range(n):
        if population[i] not in train_population:
            test_population.append(population[i])


    return train_population, test_population


def select(train_population, test_population):
    n = len(train_population)

    if n == 0:
        return 0
    
    m = len(test_population)

    #get the maximum value of the train population
    max_train = max(train_population)

    #get the first element of the test population that is greater than the maximum value of the train population
    for i in range(m):
        if test_population[i] > max_train:
            return i
        
    #else return the last element of the test population
    return m - 1


def select_top_k(population, k):
    return sorted(population, reverse=True)[:k]


def isSuccessful(population, test_population, k, selection):
    top_k = select_top_k(population, k)

    if test_population[selection] in top_k:
        return True
    else:
        return False
    


if __name__ == '__main__':

    n = int(input("Enter the size of the population: "))
    criterion = [1,3,5,10]
    epochs = int(input("Enter the number of epochs: "))

    for s in criterion:
        success_count = [0 for _ in range(0, n)]
        for _ in range(0, epochs):
            population = random_array(n)

            for m in range(0, n):
                # print("m = ", m)
                train_population, test_population = train_test_split(population, m)
                
                selection = select(train_population, test_population)

                if isSuccessful(population, test_population, s, selection):
                    success_count[m] += 1

        
        #plot the graph
        plt.plot(range(0, n), [s_count/epochs for s_count in success_count], label = "s = " + str(s) + " , n = " + str(n) + " , epochs = " + str(epochs))
        plt.xlabel("m")
        plt.ylabel("Success Rate")
        plt.show()

        #print the maximum success rate
        print("Maximum success rate: ", max(success_count)/epochs*100.0)
        print("m = ", success_count.index(max(success_count)))



    