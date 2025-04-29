import numpy as np
import matplotlib.pyplot as plt

def codonHistogram(gene):
    codonCounter = dict()
    nCodons = len(gene) - 2
    
    i = 0
    #loop trough each character from 0 until the end of the string peaking 3 characters ahead
    #making sure that we dont peek off the end of the string
    while(i < len(gene) - 3):
        codon = gene[i:i+3]
        if(codon not in codonCounter):
            codonCounter[codon] = 1
        else:
            codonCounter[codon] += 1
        i += 1
    
    for codon in codonCounter:
        codonCounter[codon] /= nCodons
    
    return codonCounter

def histogramPlotter(histo, title, fileName=None):
    plt.close()
    plt.figure(figsize=(14,6))
    plt.bar(range(len(histo)), histo.values())
    
    #done by querying chatGPT with "explain the xticks options" 
    #followed by "list all the **kwargs and their documentation"
    plt.xticks(ticks=range(len(histo)), labels=list(histo), rotation='vertical')
    
    plt.xlabel('Codon')
    plt.ylabel('Frequency')
    plt.grid()
    plt.title(title)
    
    if(fileName != None): plt.savefig(fileName, bbox_inches='tight') #for testing purposes
    else: plt.show()

def baseDensity(geneStr, nWind=200):
    aCount = np.zeros((len(geneStr)-nWind))
    tCount = np.zeros((len(geneStr)-nWind))
    cCount = np.zeros((len(geneStr)-nWind))
    gCount = np.zeros((len(geneStr)-nWind))
    
    for i in range(len(geneStr)-nWind):
        aCount[i] = geneStr[i:i+nWind].count('A')/nWind
        tCount[i] = geneStr[i:i+nWind].count('T')/nWind 
        cCount[i] = geneStr[i:i+nWind].count('C')/nWind 
        gCount[i] = geneStr[i:i+nWind].count('G')/nWind 
    res = (aCount,tCount,cCount,gCount)
    return(res)

def baseDensityPlotter(dA, dT, dC, dG, title, fileName=None):
    plt.close()
    x = range(len(dA))
    plt.figure(figsize=(14,6))
    plt.plot(x, dA, label='A', color='blue')
    plt.plot(x, dT, label='T', color='orange')
    plt.plot(x, dC, label='C', color='green')
    plt.plot(x, dG, label='G', color='red')
    plt.title(title)
    plt.legend()
    plt.xlabel('Sequence position')
    plt.ylabel('Fraction per Window')
    plt.grid()
        
    if(fileName != None): plt.savefig(fileName, bbox_inches='tight') #for testing purposes
    else: plt.show()
    
    
def klDivergence(hP, hQ):
    sum = 0
    for codon in hP.keys():
        sum += hP[codon] * np.log((hP[codon]/hQ[codon]))
    return sum


if __name__ == "__main__":
    with open('seqSC2.txt', 'r') as geneFile:
        scov2 = geneFile.readline()
    
    histSC2 = codonHistogram(scov2)
    #histogramPlotter(histSC2, 'test')
    histogramPlotter(histSC2, 'Codon frequency in SARS-CoV-2', 'histoSC2.png')
    
    dA,dT,dC,dG = baseDensity(scov2, nWind=200)
    baseDensityPlotter(dA, dT, dC, dG, 'Density of base pairs through gene sequence', 'density.png')

    with open('seqA.txt', 'r') as geneFile:
        seqA = geneFile.readline()
        
    with open('seqB.txt', 'r') as geneFile:
        seqB = geneFile.readline()
        
    histA = codonHistogram(seqA)
    histogramPlotter(histA, 'Codon frequency in Sequence A', 'histoA.png')
    histB = codonHistogram(seqB)
    histogramPlotter(histB, 'Codon frequency in Sequence B', 'histoB.png')
    
    print("Kullback-Leibler between seqA and SARS-CoV-2:", klDivergence(histA, histSC2))
    print("Kullback-Leibler between seqB and SARS-CoV-2:", klDivergence(histB, histSC2))
