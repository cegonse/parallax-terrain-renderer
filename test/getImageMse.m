clc;

for tl = 1:64
    disp(strcat('Level: ', num2str(tl)));
    capture_1_e = imread(strcat('capture_', num2str(tl),'_0.tiff'));
    capture_2_e = imread(strcat('capture_', num2str(tl),'_1.tiff'));
    capture_3_e = imread(strcat('capture_', num2str(tl),'_2.tiff'));
    capture_4_e = imread(strcat('capture_', num2str(tl),'_3.tiff'));
    capture_5_e = imread(strcat('capture_', num2str(tl),'_4.tiff'));
    capture_6_e = imread(strcat('capture_', num2str(tl),'_5.tiff'));
    capture_7_e = imread(strcat('capture_', num2str(tl),'_6.tiff'));
    capture_8_e = imread(strcat('capture_', num2str(tl),'_7.tiff'));
    capture_9_e = imread(strcat('capture_', num2str(tl),'_8.tiff'));
    capture_10_e = imread(strcat('capture_', num2str(tl),'_9.tiff'));
    capture_11_e = imread(strcat('capture_', num2str(tl),'_10.tiff'));
    capture_12_e = imread(strcat('capture_', num2str(tl),'_11.tiff'));
    capture_13_e = imread(strcat('capture_', num2str(tl),'_12.tiff'));
    capture_14_e = imread(strcat('capture_', num2str(tl),'_13.tiff'));
    capture_15_e = imread(strcat('capture_', num2str(tl),'_14.tiff'));

    capture_1_o = imread('../Base Data/capture_64_0.tiff');
    capture_2_o = imread('../Base Data/capture_64_1.tiff');
    capture_3_o = imread('../Base Data/capture_64_2.tiff');
    capture_4_o = imread('../Base Data/capture_64_3.tiff');
    capture_5_o = imread('../Base Data/capture_64_4.tiff');
    capture_6_o = imread('../Base Data/capture_64_5.tiff');
    capture_7_o = imread('../Base Data/capture_64_6.tiff');
    capture_8_o = imread('../Base Data/capture_64_7.tiff');
    capture_9_o = imread('../Base Data/capture_64_8.tiff');
    capture_10_o = imread('../Base Data/capture_64_9.tiff');
    capture_11_o = imread('../Base Data/capture_64_10.tiff');
    capture_12_o = imread('../Base Data/capture_64_11.tiff');
    capture_13_o = imread('../Base Data/capture_64_12.tiff');
    capture_14_o = imread('../Base Data/capture_64_13.tiff');
    capture_15_o = imread('../Base Data/capture_64_14.tiff');

    % Get MSE for all screenshots
    mse1 = mse(capture_1_e, capture_1_o);
    mse2 = mse(capture_2_e, capture_2_o);
    mse3 = mse(capture_3_e, capture_3_o);
    mse4 = mse(capture_4_e, capture_4_o);
    mse5 = mse(capture_5_e, capture_5_o);
    mse6 = mse(capture_6_e, capture_6_o);
    mse7 = mse(capture_7_e, capture_7_o);
    mse8 = mse(capture_8_e, capture_8_o);
    mse9 = mse(capture_9_e, capture_9_o);
    mse10 = mse(capture_10_e, capture_10_o);
    mse11 = mse(capture_11_e, capture_11_o);
    mse12 = mse(capture_12_e, capture_12_o);
    mse13 = mse(capture_13_e, capture_13_o);
    mse14 = mse(capture_14_e, capture_14_o);
    mse15 = mse(capture_15_e, capture_15_o);

    tmse = [mse1 mse2 mse3 mse4 mse5 mse6 mse7 mse8 mse9 mse10 mse11 mse12 mse13 mse14 mse15];
    autolod_mse(tl,:) = tmse;
end

