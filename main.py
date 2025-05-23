import torch
from utils.cal_cosine import cal_cosine_similarity  
from src.model import SentenceEncoder

def pool_embedding(emb):
    return torch.mean(emb, dim=0)  # 对序列维度取平均

if __name__ == "__main__":
    encoder = SentenceEncoder()
    print(encoder.model)
    dataset=['请告诉我关于羊这个生物的一些信息',
             '请告诉我关于狼这个生物的一些信息',
             '介绍一下羊这个生物',
             '介绍一下狼这个生物'              ]
    embeddings = encoder.encode_batch(dataset)
    print(f"Embedding shape: {embeddings[0].shape}") 
    print(f"Embedding shape: {embeddings[2].shape}")
    pooled_embeddings = [pool_embedding(emb) for emb in embeddings] 
    print(f"1和2的相似度： {cal_cosine_similarity(pooled_embeddings[0], pooled_embeddings[1])}")
    print(f"1和3的相似度： {cal_cosine_similarity(pooled_embeddings[0], pooled_embeddings[2])}")
    print(f"1和4的相似度： {cal_cosine_similarity(pooled_embeddings[0], pooled_embeddings[3])}")