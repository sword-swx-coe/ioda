import matplotlib.pyplot as plt
days = ["06","07","08","09","10"]
day = days[0]
alts = []
f = open("mp202301" + day + ".oupt")
lines = f.readlines()
f.close()
Vns = []
Ves = []
Vvs = []
for ln in range(len(lines)-1):
    VNs = []
    VEs = []
    VVs = []
    line = lines[ln+1].split()
    alts.append(float(line[0]))
    VNs.append(float(line[1]))
    VEs.append(float(line[2]))
    VVs.append(float(line[3]))

    Vns.append(VNs)
    Ves.append(VEs)
    Vvs.append(VVs)
for s in range(len(days)-1):
    day = days[s+1]
    f = open("mp202301" + day + ".oupt")
    lines = f.readlines()
    f.close()
    for ln in range(len(lines)-1):
        
        line = lines[ln+1].split()
        Vns[ln].append(float(line[1]))
        Ves[ln].append(float(line[2]))
        Vvs[ln].append(float(line[3]))

Ds = ["North", "East", "Vertical"]
Vs = [Vns,Ves,Vvs]
for d in range(3):
    fig, ax1 = plt.subplots()
    La = len(alts)-1
    ticks = [alts[i] for i in [0,0,La//4, La//2, 3*La//4,La]]
    im = ax1.imshow(Vs[d],vmin=-20, vmax=20, aspect=1/8)
    plt.colorbar(im)
    ax1.invert_yaxis()
    #ax1.set_title(f"Wind Velocity {Ds[d]} (m/s)\nRio Grande Observatory")
    ax1.set_yticks([0,0,La//4, La//2, 3*La//4,La],ticks)
    ax1.set_xlabel("Days since 01/06/2023")
    ax1.set_ylabel("Altitude (km)")
    plt.show()
