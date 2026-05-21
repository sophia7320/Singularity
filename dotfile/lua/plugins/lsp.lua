return {
	{
		"mason-org/mason.nvim",
		lazy = false,
		opts = {
			ui = {
				border = "rounded",
				icons = {
					package_installed = "✓", -- 已安装图标
					package_pending = "➜", -- 安装中图标
					package_uninstalled = "✗",
				},
			},
			ensure_installed = {
				"stylua",
				"ruff",
				"prettierd",
				"goimports",
				"luacheck",
				"clang-format",
			},
		},
	},
	{
		"neovim/nvim-lspconfig",
		dependencies = { "mason-org/mason.nvim" },
	},
	{
		"mason-org/mason-lspconfig.nvim",
		dependencies = {
			"mason-org/mason.nvim",
			"neovim/nvim-lspconfig",
		},
		opts = {
			ensure_installed = {
				"lua_ls",
				"ts_ls",
				"rust_analyzer",
				"pyright",
				"clangd",
			},
			automatic_enable = true,

			handlers = {
				["lua_ls"] = function()
					vim.lsp.config("lua_ls", {
						settings = {
							Lua = {
								diagnostics = { globals = { "vim" } },
							},
						},
					})
				end,
			},
		},
	},
	{
		"stevearc/conform.nvim",
		event = "BufWritePre",
		cmd = "ConformInfo",

		keys = {
			"<leader>f",

			mode = { "n", "x" },
			callback = function()
				require("conform").format({
					async = true,
					lsp_format = "fallback",
				})
			end,
			desc = "[F]ormat buffer",
		},

		opts = {
			formatters_by_ft = {
				lua = { "stylua" },
				python = { "ruff_format" },
				javascript = { "prettierd", "prettier", stop_after_first = true },
				typescript = { "prettierd", "prettier", stop_after_first = true },
				go = { "goimports", "gofmt" },
				--rust = { "rustfmt" },
				cpp = { "clang-format" },
				c = { "clang-format" },
			},
			format_on_save = { timeout_ms = 500, lsp_format = "fallback" },
		},
	},
	{
		"mfussenegger/nvim-lint",
		event = { "BufReadPre", "BufNewFile" },

		config = function()
			require("lint").linters_by_ft = {
				python = { "ruff" },
				javascript = { "eslint_d" },
				typescript = { "eslint_d" },
				lua = { "luacheck" },
			}

			vim.api.nvim_create_autocmd("BufWritePost", {
				desc = "running linter automaticly after saving",
				callback = function()
					require("lint").try_lint()
				end,
			})
		end,
	},
}
