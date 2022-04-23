all:
	g++ DBSCAN.cpp -o DBSCAN -IC:/Python/include -LC:/Python/libs -lpython310 -DWITHOUT_NUMPY