
from typing import List, Tuple
from sklearn.cluster import KMeans
import numpy as np
import sys

NODE_POSITIONS = np.asarray([
    (340, 160),
    (396, 325),
    (365, 425),
    (300, 240),
    (230, 230),
    (145, 125),
    (415, 230),
    (250, 160),
    (325, 185),
    (362, 350),
    (295, 370),
    (300, 335),
    (367, 198),
    (335, 280),
    (317, 185)
])

def main(argv: List[str] = ["1"]) -> None:
    kMeans = KMeans(n_clusters=int(argv[1]))
    kMeans.fit(NODE_POSITIONS)
    gateway_positions = np.asarray(kMeans.cluster_centers_, dtype=np.int32)
    
    for gateway in gateway_positions.tolist():
        print(f"{gateway[0]} {gateway[1]}")

if __name__ == "__main__":
    main(sys.argv)