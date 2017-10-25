if [ ! -d data/L10 ]; then
  mkdir data/L10
fi
if [ ! -d data/L10/beta1 ]; then
  mkdir data/L10/beta1
fi
if [ ! -d data/L10/beta1/freq0 ]; then
  mkdir data/L10/beta1/freq0
fi
./reca data/L10/beta1/freq0/0 10 1 0 100000
./reca data/L10/beta1/freq0/1 10 1 0 100000
./reca data/L10/beta1/freq0/2 10 1 0 100000
if [ ! -d data/L10/beta1/freq1 ]; then
  mkdir data/L10/beta1/freq1
fi
./reca data/L10/beta1/freq1/0 10 1 1 100000
./reca data/L10/beta1/freq1/1 10 1 1 100000
./reca data/L10/beta1/freq1/2 10 1 1 100000
if [ ! -d data/L10/beta1/freq5 ]; then
  mkdir data/L10/beta1/freq5
fi
./reca data/L10/beta1/freq5/0 10 1 5 100000
./reca data/L10/beta1/freq5/1 10 1 5 100000
./reca data/L10/beta1/freq5/2 10 1 5 100000
