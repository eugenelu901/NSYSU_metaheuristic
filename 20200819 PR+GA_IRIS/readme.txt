GA 輪盤法平均最佳解為 44.1% 有微微收斂但是de了很久還是不知道為什麼收斂如此緩慢
GA 競爭法平均最佳解為 63.1% 收斂比較明顯，但還是達不到預想的結果

競爭法看起來比較正常一點，但都還是不如預期的結果，會再debug跟調整參數進行嘗試

Kmeans+GA 輪盤法 平均最佳解為88.3%
Kmeans+GA 輪盤法 平均最佳解為92.6%

加入kmeans之後，3個iteration進行初始化後就達到很不錯的準確率了，但也停止收斂了。