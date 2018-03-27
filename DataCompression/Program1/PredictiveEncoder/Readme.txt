gcc PredictiveEncoder.c -o PredictiveEncoder
./PredictiveEncoder DCabforCutie44.1.16Ascii.txt DCabforCutieEncoded.txt

gcc PredictiveDecoder.c -o PredictiveDecoder
./PredictiveDecoder DCabforCutieEncoded.txt DCabforCutieDecoded.txt

