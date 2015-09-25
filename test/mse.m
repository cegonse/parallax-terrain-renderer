function [ error ] = mse( original, estimated )
%MSE Summary of this function goes here
%   Detailed explanation goes here

    original = double(original(:));
    estimated = double(estimated(:));
    
    error = original - estimated;
    
    if error == 0
        error = 0;
        return
    else
        error = error ./ max(error);
        error = error .^ 2;
        error = mean(error);
    end
end

