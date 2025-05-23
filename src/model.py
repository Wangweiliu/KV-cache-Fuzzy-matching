import torch
from transformers import BertTokenizer, BertModel
from typing import List

class SentenceEncoder:
    def __init__(self, model_name: str = 'bert-base-chinese'):
        self.tokenizer = BertTokenizer.from_pretrained(model_name)
        self.model = BertModel.from_pretrained(model_name)
        self.model.eval()
        
    def encode(self, sentence: str) -> torch.Tensor:
        inputs = self.tokenizer(
            sentence,
            return_tensors="pt",
            padding=True,
            truncation=True
        )
        
        with torch.no_grad():
            # 获取embeddings层的输出
            embedding_output = self.model.embeddings(
                input_ids=inputs['input_ids'],
                token_type_ids=inputs['token_type_ids'],
                position_ids=None
            )
            # 返回每个token的embedding (包含了位置编码和类型编码的信息)
            return embedding_output.squeeze(0)
    
    def encode_batch(self, sentences: List[str]) -> List[torch.Tensor]:
        return [self.encode(sent) for sent in sentences]
