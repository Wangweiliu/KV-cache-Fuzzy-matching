import torch
from utils.cal_cosine import cal_cosine_similarity  

if __name__ == "__main__":
    v1 = torch.tensor([1.0, 2.0, 3.0])
    v2 = torch.tensor([4.0, 5.0, 6.0])
    sim = cal_cosine_similarity(v1, v2)  
    print(sim)