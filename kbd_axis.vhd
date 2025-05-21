library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity kbd_axis is
	generic (
		DATA_WIDTH : integer := 8
	);
	port (
		-- external interface signals
		ps2_clk_in: in std_logic;
		ps2_data_in: in std_logic;
		-- AXI stream interface signals
		axis_aclk_i : in std_logic;
		axis_aresetn_i : in std_logic;
		-- master axi stream interface
		m_axis_tready_i : in std_logic;
		m_axis_tvalid_o : out  std_logic;
		m_axis_tdata_o : out  std_logic_vector(DATA_WIDTH-1 downto 0)
	);
end kbd_axis;

architecture kbd_arch of kbd_axis is
	signal tvalid : std_logic;
	type state_type is (st_idle, st_data);
	signal state : state_type;
	signal data : std_logic_vector(DATA_WIDTH-1 downto 0);

	signal ps2_valid  : std_logic;
	signal ps2_code_o : std_logic_vector(DATA_WIDTH-1 downto 0);
begin
	m_axis_tvalid_o <= tvalid;
	data <= ps2_code_o;

	ps2_keyboard: entity work.ps2_keyboard
	port map(
		clk 	     => axis_aclk_i,
		ps2_clk      => ps2_clk_in,
		ps2_data     => ps2_data_in,
		ps2_code_new => ps2_valid,
		ps2_code     => ps2_code_o
	);

	kbd_process: process(axis_aclk_i) is
	begin
		if axis_aresetn_i = '0' then
			tvalid <= '0';
		elsif axis_aclk_i'event and axis_aclk_i = '1' then
			case state is
				when st_idle =>
					tvalid <= '1';
					if m_axis_tready_i = '1' and ps2_valid = '1' then
						m_axis_tdata_o <= data;
						state <= st_data;
					end if;
				when st_data =>
					tvalid <= '0';
					state <= st_idle;
			end case;
		end if;
	end process;

end kbd_arch;