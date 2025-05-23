import torch.nn.functional as F
import torch

def cal_cosine_similarity(v1:torch.Tensor, v2:torch.Tensor) -> float:
    dot_product=torch.dot(v1,v2)
    norm_v1=torch.linalg.norm(v1)
    norm_v2=torch.linalg.norm(v2)
    # cos_sim = F.cosine_similarity(v1, v2, dim=0)
    return float(dot_product/(norm_v1 * norm_v2))