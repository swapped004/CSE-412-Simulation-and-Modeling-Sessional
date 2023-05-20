import random

#probability of 0, 1, 2, 3 neutrons
p0 = 0.4825
p1 = 0.2126 * pow(0.5893, 0)
p2 = 0.2126 * pow(0.5893, 1)
p3 = 0.2126 * pow(0.5893, 2)

p0 += 1-(p0+p1+p2+p3)


print("p0 = ", p0)
print("p1 = ", p1)
print("p2 = ", p2)
print("p3 = ", p3)

#cumulative probability
p1 += p0
p2 += p1
p3 += p2


def calculate_neutrons():
    p = random.uniform(0,1)

    global no_of_neutrons

    if p <= p0:
        no_of_neutrons += 0
    
    elif p <= p1:
        no_of_neutrons += 1

    elif p <= p2:
        no_of_neutrons += 2

    else:
        no_of_neutrons += 3

def tree(i):
    global n
    if i == n-1:
      calculate_neutrons()
      return

    p = random.uniform(0,1)

    if p <= p0:
        pass
        
    elif p <= p1:
        tree(i+1)

    elif p <= p2:
        tree(i+1)
        tree(i+1)

    else:
        tree(i+1)
        tree(i+1)
        tree(i+1)    
   



if __name__ == '__main__':
   max_gen = int(input("Enter max generation: "))
   epochs = int(input("Enter the number of epochs: "))

   out = open("5_b_output.txt", "w")
   

   for i in range(max_gen):
        global n
        n = i+1
        neutron_cnt = [0 for _ in range(5)]
        for _ in range(epochs):
           global no_of_neutrons
           no_of_neutrons = 0
           tree(0)

           if no_of_neutrons == 0:
               neutron_cnt[0] += 1

           elif no_of_neutrons == 1:
               neutron_cnt[1] += 1

           elif no_of_neutrons == 2:
               neutron_cnt[2] += 1

           elif no_of_neutrons == 3:
               neutron_cnt[3] += 1

           else:
               neutron_cnt[4] += 1
        
        print("For ", n, " generations: ")
        print("0 neutrons: ", neutron_cnt[0]/epochs)
        print("1 neutrons: ", neutron_cnt[1]/epochs)
        print("2 neutrons: ", neutron_cnt[2]/epochs)
        print("3 neutrons: ", neutron_cnt[3]/epochs)
        print("4 neutrons: ", neutron_cnt[4]/epochs)

        out.write("For " + str(n) + " generations: \n")
        out.write("0 neutron: " + str(neutron_cnt[0]/epochs) + "\n")
        out.write("1 neutrons: " + str(neutron_cnt[1]/epochs) + "\n")
        out.write("2 neutrons: " + str(neutron_cnt[2]/epochs) + "\n")
        out.write("3 neutrons: " + str(neutron_cnt[3]/epochs) + "\n")
        out.write("4 neutrons: " + str(neutron_cnt[4]/epochs) + "\n\n")


