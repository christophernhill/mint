from matplotlib.pylab import loglog, xlabel, title, ylabel, show, legend

num_cells  = [ 96, 384, 1536, 6144, 24576, 98304, 393216, 1572864, 6291456, 25165824, ]

errorA    = [ 0.416924563944772453, 0.277262287289998299, 0.134392919718887172, 0.245031586846088972, 0.023023834645704455, 0.001067835592466995, 0.000456465396775213, 0.000226539494076972, 0.000067498240687275, 0.000023964248374433, ]
relErrorA = [ 3.744349175577645461, 2.490059129666913229, 1.206966587412551029, 2.200599100030885324, 0.206774279401055278, 0.009590102541513715, 0.004099460621661511, 0.002034523847329007, 0.000606193550888545, 0.000215220021567336, ]


errorB     = [ 0.175030240695008721, 0.067534759428552249, 0.010995970088874651, 0.002574676044375224, 0.000951489375614623, 0.000245604849200998, 0.000080200459886992, 0.000003588723842096, 0.000001701284630951, 0.000000757737656398, ]
relErrorB = [ 0.160556178597940052, 0.061949997059501043, 0.010086662341528224, 0.002361764145276266, 0.000872806307747826, 0.000225294645520990, 0.000073568312024952, 0.000003291955629166, 0.000001560597517139, 0.000000695076816486, ]

errorC     = [ 0.000000000016636444, 0.000000000002789586, 0.000000000030264769, 0.000000000004878274, 0.000000000020148045, 0.000000000012408988, 0.000000000001405252, 0.000000000001255601, 0.000000000000955607, 0.000000000001762757, ]
relErrorC = [ 23971.821325648412312148, 4019.576368876080778136, 43609.177233429392799735, 7029.213256484149496828, 29031.765129682993574534, 17880.386167146971274633, 2024.858789625360259379, 1809.223342939481199210, 1376.955331412103760158, 2539.995677233428978070, ]

loglog(num_cells, errorA, 'm-')
loglog(num_cells, errorB, 'm--')

legend(['A', 'B'])
loglog(num_cells, errorB, 'ks', markerfacecolor='None')
loglog(num_cells, errorA, 'ko', markerfacecolor='None')

alpha = -1. # convergence rate in number of cells
n0, n1 = num_cells[0], num_cells[-1]
e0 = 10.0
c = n0**(-alpha) * e0
e1 = c * n1**alpha
loglog([n0, n1], [e0, e1], 'k--')


xlabel('Number of horizontal grid cells')
ylabel('Error')
title('Flux computation error on cubed sphere grid')
show()