%% Converts the starting sequence to a float to check stuff
typecast(uint8([254 254 254 193]),'single')

%% See what 2*pi is:
a = single(2*pi);
byts = typecast(a,'uint8')

%% See what -2*pi is:
a = single(-2*pi);
byts = typecast(a,'uint8')
hex = dec2hex(byts)

a = single(pi/6);
byts = typecast(a,'uint8')
hex = dec2hex(byts)

a = single(0);
byts = typecast(a,'uint8')
hex = dec2hex(byts)

a = single(0.5);
byts = typecast(a,'uint8')
hex = dec2hex(byts)


