set table "paper.gauss1.table"; set format "%.5f"
set samples 10000; set parametric; plot [t=-3:3] 7.5+1.5*exp(-(t-0.75)**2)+1.5*exp(-(t+0.75)**2),t
