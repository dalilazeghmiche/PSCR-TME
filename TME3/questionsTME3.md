# Questions - TME 3 : Threads

Instructions : copiez vos réponses dans ce fichier (sous la question correspondante). A la fin de la séance, commitez vos réponses.

## Question 1.

```
cd build-release && ./TME3 ../WarAndPeace.txt freqstd && ./TME3 ../WarAndPeace.txt freqstdf && ./TME3 ../WarAndPeace.txt freq && check.sh *.freq


traces pour les 3 modes, invocation a check qui ne rapporte pas d'erreurcd

PS C:\Users\ESKMT\git\PSCR-TME\TME3\build-release> .\TME3.exe ..\WarAndPeace.txt freqstd
>> .\TME3.exe ..\WarAndPeace.txt freqstdf
>> .\TME3.exe ..\WarAndPeace.txt freq
>>
Preparing to parse ..\WarAndPeace.txt (mode=freqstd N=4), containing 3300667 bytes
Total runtime (wall clock) : 450 ms
Preparing to parse ..\WarAndPeace.txt (mode=freqstdf N=4), containing 
3300667 bytes
Total runtime (wall clock) : 447 ms
Preparing to parse ..\WarAndPeace.txt (mode=freq N=4), containing 3300667 bytes
Total runtime (wall clock) : 456 ms
PS C:\Users\ESKMT\git\PSCR-TME\TME3\build-release>
```

## Question 2.

start vaut : début de la tranche du fichier, généralement i * chunkSize (0 pour le premier thread)

end vaut end vaut : fin de la tranche, généralement (i+1) * chunkSize, ou fileSize pour le dernier thread

Code des lambdas :
// Mode freqstdf
processRange(filename, start, end, [&](const std::string& word){
    stdMap[word]++; // met à jour le std::unordered_map
});

// Mode freq
processRange(filename, start, end, [&](const std::string& word){
    hashMap.insertOrIncrement(word); // met à jour le HashMap maison
});

```
code
```

Accès identifiés :
word : lecture seule (const référence)

stdMap ou hashMap : modifié directement via capture par référence [&]
## Question 3.

Continuez de compléter ce fichier avec vos traces et réponses.

...
o/textfile] [mode] [num threads]
PS C:\Users\ESKMT\git\PSCR-TME\TME3> cd build-release
PS C:\Users\ESKMT\git\PSCR-TME\TME3\build-release> .\TME3.exe ..\WarAndPeace.txt partition 4
>> 
Preparing to parse ..\WarAndPeace.txt (mode=partition N=4), containing 3300667 bytes
Unknown mode 'partition'. Supported modes: freqstd, freq, freqstdf    
PS C:\Users\ESKMT\git\PSCR-TME\TME3\build-release>

Q3:
Identifiez si cela résout partiellement des problèmes (lesquels ?):
L’utilisation de std::atomic<size_t> pour le compteur total_words résout partiellement les problèmes :

✅ Résolu : le compteur global total_words est maintenant correct et déterministe, même avec plusieurs threads en parallèle.

⚠️ Non résolu : les accès concurrents au HashMap maison (hm.incrementFrequency(word)) ne sont toujours pas protégés. Il peut donc y avoir des data races sur les fréquences des mots, ce qui peut produire des résultats incorrects pour certains mots.

En résumé : seul le problème du comptage total des mots est corrigé, les erreurs sur les fréquences individuelles persistent.

Q7:
Les problèmes de comptage des valeurs existantes sont résolus.

Les problèmes liés à l’ajout simultané de nouveaux mots ne sont pas complètement résolus.






